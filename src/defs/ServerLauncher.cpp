/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerLauncher.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelha <eabdelha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/14 11:27:06 by eabdelha          #+#    #+#             */
/*   Updated: 2022/12/27 08:21:46 by eabdelha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../classes/ServerLauncher.hpp"
#include "../../ztrash/debug.h"

/******************************************************************************/
/*                            construct-destruct                              */
/******************************************************************************/

ServerLauncher::ServerLauncher(std::vector<ServerSet>* _v) : _server_set(_v)
{
    _kq = ::kqueue();
    if (_kq < 0)
        throw err_strerror(std::string("kqueue: "));
}
ServerLauncher::~ServerLauncher()
{
}

/******************************************************************************/
/*                            core server loop                                */
/******************************************************************************/

void ServerLauncher::core_server_loop(void)
{
    size_t                      nev;
    std::vector<struct kevent>  o_events;
    
    o_events.reserve(NB_OEVENTS);
    for (;;)
    {
        nev =  this->kevent(o_events);
        
        for (size_t i = 0; i < nev; ++i)
        {
            if (o_events[i].filter == EVFILT_READ)
            {
                if (!o_events[i].udata)
                    this->accept(o_events[i].ident, o_events[i].data);
                else
                {
                    try
                    {
                        ((RecvHandler*)o_events[i].udata)->operator()(o_events[i].ident, o_events[i].data);
                    }
                    catch(const std::exception& e)
                    {
                        std::cerr << e.what() << '\n';
                        _rhandl.erase(o_events[i].ident);
                        close(o_events[i].ident);
                    }
                    if (((RecvHandler*)o_events[i].udata)->get_is_done())
                    {
                        toggle_event(o_events[i].ident, EVFILT_READ, EV_DELETE);
                        _shandl[o_events[i].ident].set_response(&_responses[o_events[i].ident]);
                        toggle_event(o_events[i].ident, EVFILT_WRITE, EV_ADD | EV_CLEAR, &_shandl[o_events[i].ident]);
                        _rhandl.erase(o_events[i].ident);
                    }
                }
            }
            else if (o_events[i].filter == EVFILT_WRITE)
            {
                try
                {
                    ((SendHandler*)o_events[i].udata)->operator()(o_events[i].ident, o_events[i].data);
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                    ((SendHandler*)o_events[i].udata)->set_is_done(true);
                }
                if (((SendHandler*)o_events[i].udata)->get_is_done())
                {
                        toggle_event(o_events[i].ident, EVFILT_WRITE, EV_DELETE);
                        _shandl.erase(o_events[i].ident);
                        _responses.erase(o_events[i].ident);
                        close(o_events[i].ident);
                }
            }
        }
    }
}

/******************************************************************************/
/*                            system call wrappers                            */
/******************************************************************************/

void ServerLauncher::launch_routines(void)
{
    std::map<int, int> pair_port_fd;
    
    for (size_t i = 0; i < _server_set->size(); ++i)
    {
        if (pair_port_fd.find((*_server_set)[i]._port) != pair_port_fd.end())
        {
            _pair_fd_servers[pair_port_fd[(*_server_set)[i]._port]].push_back(&(*_server_set)[i]);
            continue;
        }
        this->socket();
        this->setsockopt();
        this->fcntl();
        this->bind((*_server_set)[i]);
        this->listen();
        pair_port_fd[(*_server_set)[i]._port] = _sock_fds.back();
        _pair_fd_servers[_sock_fds.back()].push_back(&(*_server_set)[i]);
    }
    enable_socket_monitoring();
    // std::map<int, std::vector<ServerSet*> >::iterator it;
    // for (it = _pair_fd_servers.begin(); it != _pair_fd_servers.end(); ++it)
    // {
    //     std::cout << it->first << ": \n";
    //     for (size_t i  = 0; i < it->second.size(); ++i)
    //         std::cout << it->second[i]->g_port() << " ";
    //     std::cout << '\n';
    // }
    // exit (0);
}

void ServerLauncher::socket(void)
{
    
    _sock_fds.push_back(::socket(AF_INET, SOCK_STREAM, 0));
    if (_sock_fds.back() < 0)
        throw err_strerror(std::string("socket: "));
}
void ServerLauncher::setsockopt(void)
{
    int enable = 1;
    if (::setsockopt(_sock_fds.back(), SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
        throw err_strerror(std::string("setsockopt: "));
    if (::setsockopt(_sock_fds.back(), SOL_SOCKET, SO_NOSIGPIPE, &enable, sizeof(int)) < 0)
        throw err_strerror(std::string("setsockopt: "));
}
void ServerLauncher::fcntl(void)
{
    if (::fcntl(_sock_fds.back(), F_SETFL,  O_NONBLOCK) < 0)
        throw err_strerror(std::string("fcntl: "));
}
void ServerLauncher::bind(ServerSet &serv)
{
    struct sockaddr_in addr;
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(serv._port);
    addr.sin_addr.s_addr = inet_addr(serv._address.data());
    if (::bind(_sock_fds.back(), (struct sockaddr*)&addr, sizeof(addr)) < 0)
        throw err_strerror(std::string("bind: "));
}
void ServerLauncher::listen(void)
{
    if (::listen(_sock_fds.back(), LISN_BACKLOG) < 0)
        throw err_strerror(std::string("listen: "));
}

int ServerLauncher::kevent(std::vector<struct kevent> &o_events)
{
    int rc = ::kevent(_kq, NULL, 0, o_events.data(), NB_OEVENTS, 0);
    if (rc < 0)
        throw err_strerror(std::string("kevent: "));
    return (rc);
}

void ServerLauncher::accept(int fd, int ndata)
{
    std::vector<struct kevent>  i_events;

    while (ndata--)
    {
        int rc = ::accept(fd, NULL, NULL);
        if (rc < 0)
            throw err_strerror(std::string("accept: "));
        i_events.push_back((struct kevent){});
        EV_SET(&i_events.back(), rc, EVFILT_READ, EV_ADD | EV_CLEAR, 0, 0, &_rhandl[rc]);
        _rhandl[rc].set_servers(&_pair_fd_servers[fd]);
        _rhandl[rc].set_response(&_responses[rc]);
    }
    ::kevent(_kq, i_events.data(), i_events.size(), NULL, 0, 0);
}

/******************************************************************************/
/*                            utility functions                               */
/******************************************************************************/

void ServerLauncher::enable_socket_monitoring(void)
{
    std::vector<struct kevent>  i_events;
    for (size_t i = 0; i < _sock_fds.size(); ++i)
    {
        i_events.push_back((struct kevent){});
        EV_SET(&i_events.back(), _sock_fds[i], EVFILT_READ, EV_ADD | EV_CLEAR, 0, 0, NULL);
    }
    ::kevent(_kq, i_events.data(), i_events.size(), NULL, 0, 0);
}

void ServerLauncher::toggle_event(size_t fd, int16_t filter, uint16_t flags, void *udata)
{
    struct kevent event;

    EV_SET(&event, fd, filter, flags, 0, 0, udata);
    ::kevent(_kq, &event, 1, NULL, 0, 0);
}

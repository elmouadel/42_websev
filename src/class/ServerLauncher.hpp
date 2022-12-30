/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerLauncher.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelha <eabdelha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/14 11:27:26 by eabdelha          #+#    #+#             */
/*   Updated: 2022/12/29 17:58:34 by eabdelha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
#ifndef SERVER_LAUNCHER_HPP
#define SERVER_LAUNCHER_HPP

# include <map>
# include <vector>
# include <utility>
# include <cstring>
# include <errno.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/event.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <sys/socket.h>
# include "../incl/macros.hpp"
# include "../incl/utils.hpp"
# include "../incl/exceptions.hpp"
# include "../incl/MiniStructs.hpp"
# include "../incl/ServerSettings.hpp"
# include "../class/RecvHandler.hpp"
# include "../class/SendHandler.hpp"

#define RECV_HANDLER(i)  ((RecvHandler*)o_events[i].udata)->operator()
#define SEND_HANDLER(i)  ((SendHandler*)o_events[i].udata)->operator()

class ServerLauncher
{
    int                                     _kq;
    std::vector<int>                        _sock_fds;
    std::vector<ServerSet>*                 _server_set;
    std::map<int, Response>                 _responses;
    std::map<int, RecvHandler>              _rhandl;
    std::map<int, RecvHandler*>             _icgi;
    std::map<int, SendHandler>              _shandl;
    std::map<int, std::vector<ServerSet*> > _pair_fd_servers;
    

    public:
        ServerLauncher(std::vector<ServerSet>*);    
        ~ServerLauncher();
        
        void    core_server_loop(void);
        void    launch_routines(void);
        void    socket(void);
        void    setsockopt(void);
        void    fcntl(void);
        void    bind(ServerSet&);
        void    listen(void);
        int     kevent(std::vector<struct kevent>&);
        void    accept(int fd, int ndata);
        void    enable_socket_monitoring(void);
        void    toggle_event(size_t fd, int16_t filter, uint16_t flags, void *udata = nullptr);
        void    recv_event_handler(struct kevent &o_event);
        
};

#endif
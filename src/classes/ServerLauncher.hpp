/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerLauncher.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelha <eabdelha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/14 11:27:26 by eabdelha          #+#    #+#             */
/*   Updated: 2022/12/16 01:23:16 by eabdelha         ###   ########.fr       */
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
# include "../includes/macros.hpp"
# include "../includes/utils.hpp"
# include "../includes/exceptions.hpp"
# include "../includes/MiniStructs.hpp"
# include "../includes/ServerSettings.hpp"
# include "../classes/RecvHandler.hpp"
# include "../classes/SendHandler.hpp"

class ServerLauncher
{
    int                                     _kq;
    std::vector<int>                        _sock_fds;
    std::vector<ServerSet>*                 _server_set;
    std::map<int, Response>                 _responses;
    std::map<int, RecvHandler>              _rhandl;
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
        
};

#endif
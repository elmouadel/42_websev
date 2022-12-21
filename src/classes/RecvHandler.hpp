/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RecvHandler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelha <eabdelha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/14 12:07:27 by eabdelha          #+#    #+#             */
/*   Updated: 2022/12/21 09:22:46 by eabdelha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RECV_HANDLER_HPP
#define RECV_HANDLER_HPP

# include <vector>
# include <cstring>
# include <iostream>
# include <sys/socket.h>
# include "../includes/utils.hpp"
# include "../includes/exceptions.hpp"
# include "../includes/MiniStructs.hpp"
# include "../includes/ServerSettings.hpp"
# include "../classes/SendHandler.hpp"
# include "../classes/RequestParser.hpp"
# include "../classes/RequestProcessor.hpp"
# include "../classes/UploadHandler.hpp"

class RecvHandler
{

    bool                        _is_done;
    bool                        _is_head;
    size_t                      _rlength;
    Response*                   _response;
    std::string                 _rhead;
    std::string                 _rbody;
    std::vector<ServerSet*>*    _servers;
    std::vector<std::string>    _r_fields;
    std::vector<std::string>    _s_fields;
    LocationSet*                _location;
    
    public:
        RecvHandler();  
        ~RecvHandler();
        void operator()(int fd, int ndata);
        void recv_head(int fd, int &ndata);
        void recv_body(int fd, int ndata);
        void set_servers(std::vector<ServerSet*>*);
        void set_response(Response*);
        bool get_is_done(void);
        void set_is_done(bool);
        ServerSet *get_matched_server(std::vector<ServerSet*> *server_list);
        LocationSet *get_matched_location(std::vector<ServerSet*> *server_list);
};

#endif
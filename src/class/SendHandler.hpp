/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SendHandler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelha <eabdelha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/14 12:07:28 by eabdelha          #+#    #+#             */
/*   Updated: 2022/12/29 17:58:34 by eabdelha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SEND_HANDLER_HPP
#define SEND_HANDLER_HPP

# include <vector>
# include <iostream>
# include <sys/socket.h>
# include "../incl/utils.hpp"
# include "../incl/MiniStructs.hpp"
# include "../incl/ServerSettings.hpp"
# include "../class/SendHandler.hpp"

class SendHandler
{
    bool        _is_done;
    bool        _is_body;
    size_t      _wlength;
    Response*   _response;
    char*       _buf_switch;
    size_t      _buf_switch_len;;
    
    public:
        SendHandler();  
        ~SendHandler();
        void operator()(int fd, int ndata);
        void set_response(Response*);
        bool get_is_done(void);
        void set_is_done(bool);
};

#endif
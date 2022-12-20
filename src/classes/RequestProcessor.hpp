/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestProcessor.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelha <eabdelha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 22:44:40 by eabdelha          #+#    #+#             */
/*   Updated: 2022/12/21 00:29:07 by eabdelha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_PROCESSOR_HPP
#define REQUEST_PROCESSOR_HPP

# include <vector>
# include <string>
# include <cstring>
# include <iostream>
# include <unistd.h>
# include <fcntl.h>
# include <sys/mman.h>
# include <sys/stat.h>
# include <sys/errno.h>
# include "../includes/macros.hpp"
# include "../includes/exceptions.hpp"
# include "../includes/MiniStructs.hpp"
# include "../includes/ServerSettings.hpp"

class RequestProcessor 
{
    Response*                   _response;
    std::vector<std::string>*   _r_fields;
    std::vector<std::string>*   _s_fields;
    LocationSet*                _location;
    
    public:
        RequestProcessor(Response*, std::vector<ServerSet*>*, std::vector<std::string>*, std::vector<std::string>*);
        ~RequestProcessor();
        ServerSet *get_matched_server(std::vector<ServerSet*> *server_list);
        LocationSet *get_matched_location(std::vector<ServerSet*> *server_list);
        bool process_request(void);
        void get_method_handler(void);
        void get_response_body(std::string&);
        void check_is_allowed_method(void);
        void set_err_page(std::map<int, std::string> &err_page);
};

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestProcessor.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelha <eabdelha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 22:44:40 by eabdelha          #+#    #+#             */
/*   Updated: 2023/01/01 10:40:38 by eabdelha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_PROCESSOR_HPP
#define REQUEST_PROCESSOR_HPP

# include <vector>
# include <string>
# include <cstring>
# include <iostream>
# include <dirent.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/mman.h>
# include <sys/stat.h>
# include "../class/CGIExecutor.hpp"
# include "../incl/utils.hpp"
# include "../incl/macros.hpp"
# include "../incl/exceptions.hpp"
# include "../incl/MiniStructs.hpp"
# include "../incl/ServerSettings.hpp"

class RequestProcessor 
{
    Response*                   _response;
    std::vector<std::string>*   _r_fields;
    std::vector<std::string>*   _s_fields;
    LocationSet*                _location;
    
    public:
        RequestProcessor(Response*, std::vector<std::string>*, std::vector<std::string>*, LocationSet*);
        ~RequestProcessor();
        bool process_request(void);
        void get_method_handler(void);
        void get_response_body(const char*);
        void get_response_cgi_body(const char*);
        void execute_cgi(void);
        void check_is_allowed_method(void);
        void list_directory(void);
};

#endif


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIExecutor.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelha <eabdelha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/23 22:25:40 by eabdelha          #+#    #+#             */
/*   Updated: 2022/12/24 22:17:38 by eabdelha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_EXECUTOR_HPP
#define CGI_EXECUTOR_HPP

# include <map>
# include <vector>
# include <string>
# include <fcntl.h>
# include <unistd.h>
# include "../includes/ServerSettings.hpp"
# include "../includes/exceptions.hpp"
# include "../includes/macros.hpp"

class CGIExecutor
{
    std::map<int, std::string>  _env;
    std::map<int, std::string>  _args;
    std::string*                _body;
    
    public:
        CGIExecutor(std::vector<std::string>&, LocationSet&);
        ~CGIExecutor();
        void set_body(std::string *);
        void execute_cgi(void);
};


#endif
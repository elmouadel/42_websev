/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelha <eabdelha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/13 21:59:21 by eabdelha          #+#    #+#             */
/*   Updated: 2022/12/27 16:04:40 by eabdelha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

# include <string>
# include <fstream>
# include <iostream>
# include "../includes/ServerSettings.hpp"
# include "../includes/exceptions.hpp"
# include "../includes/utils.hpp"


class ConfigParser
{
    typedef void (ConfigParser::*shandler)(std::string&);
    std::map<std::string, shandler>   _assign_handl_server;
    typedef void (ConfigParser::*lhandler)(LocationSet&, std::string&);
    std::map<std::string, lhandler>  _assign_handl_location;
    
    std::ifstream           _config_file;
    std::vector<ServerSet>& _server_set;

    public:
        ConfigParser(const char*, std::vector<ServerSet>&);
        ~ConfigParser();
        void main_config_file_parser(void);
        void server_block_parser(std::string&);
        void location_block_main_parser(std::string&);
        void location_block_parser(std::string&);
        
        void name_handle(std::string&);
        void listen_handle(std::string&);
        void cgi_handle(LocationSet&, std::string&);
        void err_page_handle(LocationSet&, std::string&);
        void redirect_handle(LocationSet&, std::string&);
        void index_handle(LocationSet&, std::string&);
        void acc_mtod_handle(LocationSet&, std::string&);
        void root_handle(LocationSet&, std::string&);
        void upload_dir_handle(LocationSet&, std::string&);
        void cb_max_size_handle(LocationSet&, std::string&);
        void autoindex_handle(LocationSet&, std::string&);
        void url_path_handle(std::string&);
        void print(void);
};


#endif
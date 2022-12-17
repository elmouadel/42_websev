/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestProcessor.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelha <eabdelha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 22:44:42 by eabdelha          #+#    #+#             */
/*   Updated: 2022/12/17 10:37:30 by eabdelha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../classes/RequestProcessor.hpp"
#include "../../debug.h"

/******************************************************************************/
/*                            construct-destruct                              */
/******************************************************************************/

RequestProcessor::RequestProcessor(Response* _cv1, std::vector<ServerSet*>* _cv2, 
                    std::vector<std::string>* _cv3, std::vector<std::string>* _cv4)
    : _response(_cv1), _r_fields(_cv3), _s_fields(_cv4), _location(get_matched_location(_cv2))
    
{
}
RequestProcessor::~RequestProcessor()
{
}

ServerSet *RequestProcessor::get_matched_server(std::vector<ServerSet*> *server_list)
{

    if (server_list->size() != 1)
    {
        for (size_t i = 0; i < server_list->size(); ++i)
        {
            std::set<std::string>::iterator it;
            it = (*server_list)[i]->_server_name.find((*_r_fields)[HR_HOST]);
            if (it != (*server_list)[i]->_server_name.end())
                return ((*server_list)[i]);
        }
    }
    return ((*server_list)[0]);
}

LocationSet *RequestProcessor::get_matched_location(std::vector<ServerSet*> *server_list)
{
    ServerSet   *server;
    std::string url;

    server = get_matched_server(server_list);
    url = (*_r_fields)[HR_URL];
    while (url.length())
    {
        for (size_t i = 1; i < server->_locations.size(); ++i)
        {
            if (url == server->_locations[i]._url_path)
                return (&server->_locations[i]);
        }
        size_t pos = url.find_last_of("/");
        if (pos == std::string::npos)
            pos = 0;
        url = url.substr(0, pos);
    }
    return (&server->_locations[0]);
}

/******************************************************************************/
/*                              process request                               */
/******************************************************************************/

void RequestProcessor::process_request(void)
{
    (*_r_fields)[HR_URL] = _location->_root \
    + (*_r_fields)[HR_URL].substr(_location->_url_path.length(), (*_r_fields)[HR_URL].length());
    check_is_allowed_method();
    if ((*_r_fields)[HR_METHOD] == "GET")
        get_method_handler();
}

void RequestProcessor::get_method_handler(void)
{
    struct stat file_info;

    if (stat((*_r_fields)[HR_URL].data(), &file_info) == 0)
    {
        if (file_info.st_mode & S_IFREG) 
            get_response_body((*_r_fields)[HR_URL]);
        else if (file_info.st_mode & S_IFDIR) 
        {
            std::set<std::string>::iterator it = _location->_index.begin();
            for (; it != _location->_index.end(); ++it)
            {
                std::string str((*_r_fields)[HR_URL]);
                str.append(*it);
                if (stat(str.data(), &file_info) == 0)
                {
                    if (file_info.st_mode & S_IFREG) 
                        get_response_body(str);
                }
            }
        }
        else
            throw response_status(SC_404);
    }
    else
            throw response_status(SC_404);
}

void RequestProcessor::get_response_body(std::string &file)
{
    int         fd;
    struct stat st;
    
    fd = open(file.data(), O_RDONLY);
    if (fd == -1) 
        throw server_error(std::string("error: open: ") + ::strerror(errno));
    if (fstat(fd, &st) < 0)
        throw server_error(std::string("error: fstat: ") + ::strerror(errno));
    _response->_body_len = st.st_size;
    _response->_body = (char*)mmap(NULL, _response->_body_len, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    if (_response->_body == MAP_FAILED)
        throw server_error(std::string("error: mmap: ") + ::strerror(errno));
    _response->_is_mapped = true;
    (*_s_fields)[HS_STCODE] = SC_200;
    (*_s_fields)[HS_CNTLEN] = std::to_string(_response->_body_len);
}

/******************************************************************************/
/*                                tests checks                                */
/******************************************************************************/
void RequestProcessor::check_is_allowed_method(void)
{
    std::set<std::string>::iterator it;
    
    if (!_location->_acc_mtod.empty())
    {
        it = _location->_acc_mtod.find((*_r_fields)[HR_METHOD]);
        if (it == _location->_acc_mtod.end())
        {
            for (it = _location->_acc_mtod.begin(); it != _location->_acc_mtod.end();)
            {
                (*_s_fields)[HS_ALLOWD].append(*it);
                if (++it != _location->_acc_mtod.end())
                    (*_s_fields)[HS_ALLOWD].append(", ");
            }
                
            throw response_status(SC_405);
        }
    }
}
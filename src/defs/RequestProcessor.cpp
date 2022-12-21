/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestProcessor.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelha <eabdelha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 22:44:42 by eabdelha          #+#    #+#             */
/*   Updated: 2022/12/21 23:11:43 by eabdelha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../classes/RequestProcessor.hpp"
#include "../../debug.h"

/******************************************************************************/
/*                            construct-destruct                              */
/******************************************************************************/

RequestProcessor::RequestProcessor(Response* _cv1, std::vector<std::string>* _cv2, 
    std::vector<std::string>* _cv3, LocationSet* _cv4)
    : _response(_cv1), _r_fields(_cv2), _s_fields(_cv3), _location(_cv4)
    
{
}
RequestProcessor::~RequestProcessor()
{
}

// ServerSet *RequestProcessor::get_matched_server(std::vector<ServerSet*> *server_list)
// {

//     if (server_list->size() != 1)
//     {
//         for (size_t i = 0; i < server_list->size(); ++i)
//         {
//             std::set<std::string>::iterator it;
//             it = (*server_list)[i]->_server_name.find((*_r_fields)[HR_HOST]);
//             if (it != (*server_list)[i]->_server_name.end())
//                 return ((*server_list)[i]);
//         }
//     }
//     return ((*server_list)[0]);
// }

// LocationSet *RequestProcessor::get_matched_location(std::vector<ServerSet*> *server_list)
// {
//     ServerSet   *server;
//     std::string url;

//     server = get_matched_server(server_list);
//     url = (*_r_fields)[HR_URL];
//     while (url.length())
//     {
//         for (size_t i = 1; i < server->_locations.size(); ++i)
//         {
//             if (url == server->_locations[i]._url_path)
//                 return (&server->_locations[i]);
//         }
//         size_t pos = url.find_last_of("/");
//         if (pos == std::string::npos)
//             pos = 0;
//         url = url.substr(0, pos);
//     }
//     return (&server->_locations[0]);
// }

/******************************************************************************/
/*                              process request                               */
/******************************************************************************/

bool RequestProcessor::process_request(void)
{
    if (!(*_r_fields)[HR_CNTLEN].empty() && stoul((*_r_fields)[HR_CNTLEN]) > _location->_cb_max_size)
        throw response_status (SC_413);
    _url_path = _location->_root \
    + (*_r_fields)[HR_URL].substr(_location->_url_path.length(), (*_r_fields)[HR_URL].length());
    check_is_allowed_method();
    if ((*_r_fields)[HR_METHOD] == "GET")
    {
        get_method_handler();
        return (true);
    }
    else if ((*_r_fields)[HR_METHOD] == "POST")
        return (false);
    return (true);
}

void RequestProcessor::get_method_handler(void)
{
    struct stat file_info;

    if (stat(_url_path.data(), &file_info) == 0)
    {
        if (file_info.st_mode & S_IFREG)
        {
            get_response_body(_url_path);
            return;
        }
        else if (file_info.st_mode & S_IFDIR) 
        {
            std::set<std::string>::iterator it = _location->_index.begin();
            for (; it != _location->_index.end(); ++it)
            {
                std::string str(_url_path);
                str.append(*it);
                if (stat(str.data(), &file_info) == -1)
                    continue;
                if (file_info.st_mode & S_IFREG)
                {
                    get_response_body(str);
                    return;
                }
            }
        }
        if (file_info.st_mode & S_IFDIR && _location->_autoindex)
            list_directory();
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
/******************************************************************************/
/*                               list directory                               */
/******************************************************************************/

void RequestProcessor::list_directory(void)
{
    DIR*            dir;
    struct dirent*  entry;
    std::string     flist;

    dir = opendir(_url_path.data());
    if (!dir)
        throw server_error(std::string("error: opendir: ") + ::strerror(errno));
    if ((*_r_fields)[HR_URL].back() != '/')
        (*_r_fields)[HR_URL].push_back('/');
    flist.assign("<!DOCTYPE html><html><head></head><body>");
    flist.append("<h1>Directory listing for ");
    flist.append((*_r_fields)[HR_URL] + "</h1>");
    flist.append("<hr><ul>");
    while ((entry = readdir(dir)) != nullptr)
    {
        if (std::string(entry->d_name) == ".." || std::string(entry->d_name) == ".")
            continue;
        flist.append("<li><a href=\"");
        flist.append((*_r_fields)[HR_URL]);
        flist.append(entry->d_name);
        flist.append("\">");
        flist.append(entry->d_name);
        flist.append("</a></li>");
    }
    flist.append("</ul><hr></body></html>");
    closedir(dir);
    _response->_body = (char*)malloc(flist.size());
    _response->_body_len = flist.size();
    memcpy(_response->_body, flist.data(), flist.size());
    (*_s_fields)[HS_STCODE] = SC_200;
    (*_s_fields)[HS_CNTLEN] = std::to_string(flist.size());
}
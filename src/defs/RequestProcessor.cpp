/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestProcessor.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelha <eabdelha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 22:44:42 by eabdelha          #+#    #+#             */
/*   Updated: 2022/12/27 18:08:48 by eabdelha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../classes/RequestProcessor.hpp"
#include "../../ztrash/debug.h"

/******************************************************************************/
/*                            construct-destruct                              */
/******************************************************************************/

RequestProcessor::RequestProcessor(Response* _cv1, std::vector<std::string>* _cv2, 
    std::vector<std::string>* _cv3, LocationSet* _cv4)
    : _response(_cv1), _r_fields(_cv2), _s_fields(_cv3), _location(_cv4)
    
{
    if (!(*_r_fields)[HR_CNTLEN].empty() && stoul((*_r_fields)[HR_CNTLEN]) > _location->_cb_max_size)
        throw response_status (SC_413);
    (*_r_fields)[HR_RURL] = _location->_root + "/" \
    + (*_r_fields)[HR_URL].substr(_location->_url_path.length(), (*_r_fields)[HR_URL].length());
    check_is_allowed_method();
}
RequestProcessor::~RequestProcessor()
{
}

/******************************************************************************/
/*                              process request                               */
/******************************************************************************/

bool RequestProcessor::process_request(void)
{
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
    
    if (stat((*_r_fields)[HR_RURL].data(), &file_info) == 0)
    {
        if (file_info.st_mode & S_IFREG)
        {
            if (::is_cgi(*_r_fields, _location->_cgi))
                get_cgi_response_body();
            else
                get_response_body((*_r_fields)[HR_RURL].data());
            return;
        }
        else if (file_info.st_mode & S_IFDIR) 
        {
            std::set<std::string>::iterator it = _location->_index.begin();
            for (; it != _location->_index.end(); ++it)
            {
                std::string str((*_r_fields)[HR_RURL]);
                str.append(*it);
                if (stat(str.data(), &file_info) == -1)
                    continue;
                if (file_info.st_mode & S_IFREG)
                {
                    if (::is_cgi(*_r_fields, _location->_cgi))
                        get_cgi_response_body();
                    else
                        get_response_body(str.data());
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

void RequestProcessor::get_response_body(const char *file)
{
    mmap_file(*_response, file);
    (*_s_fields)[HS_STCODE] = SC_200;
    (*_s_fields)[HS_CNTLEN] = std::to_string(_response->_body_len);
    (*_s_fields)[HS_CTYP] = get_content_type(file);
}

void RequestProcessor::get_cgi_response_body(void)
{
    CGIExecutor cgi((*_r_fields), *_location);

    cgi.execute_cgi();
    get_response_body("/tmp/cgi_tmp_file");
    (*_s_fields)[HS_LCRLF] = "";
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

    dir = opendir((*_r_fields)[HR_RURL].data());
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
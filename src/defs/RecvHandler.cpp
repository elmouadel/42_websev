/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RecvHandler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelha <eabdelha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/14 12:10:10 by eabdelha          #+#    #+#             */
/*   Updated: 2022/12/27 18:32:02 by eabdelha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../classes/RecvHandler.hpp"
#include "../../ztrash/debug.h"

/******************************************************************************/
/*                            construct-destruct                              */
/******************************************************************************/

RecvHandler::RecvHandler() : _is_done(0), _is_head(1), _rlength(0), 
    _servers(nullptr), _location(nullptr)
{
    _rhead.resize(BUF_SIZE);
    _r_fields.resize(9);
    _s_fields.resize(6);
    _s_fields[HS_LCRLF] = "\r\n";
}
RecvHandler::~RecvHandler()
{
}

/******************************************************************************/
/*                              getters-setters                               */
/******************************************************************************/

ServerSet *RecvHandler::get_matched_server(std::vector<ServerSet*> *server_list)
{

    if (server_list->size() != 1)
    {
        for (size_t i = 0; i < server_list->size(); ++i)
        {
            std::set<std::string>::iterator it;
            it = (*server_list)[i]->_server_name.find(_r_fields[HR_HOST]);
            if (it != (*server_list)[i]->_server_name.end())
                return ((*server_list)[i]);
        }
    }
    return ((*server_list)[0]);
}

LocationSet *RecvHandler::get_matched_location(std::vector<ServerSet*> *server_list)
{
    ServerSet   *server;
    std::string url;

    server = get_matched_server(server_list);
    url = _r_fields[HR_URL];
    while (url.length() && server->_locations.size() > 1)
    {
        for (size_t i = 1; i < server->_locations.size(); ++i)
        {
            if (url == server->_locations[i]._url_path)
            {
                if (!server->_locations[i]._redirect.first.empty())
                {
                    _r_fields[HR_URL].erase(0, server->_locations[i]._url_path.length());
                    _r_fields[HR_URL].insert(0, server->_locations[i]._redirect.first);
                    
                    _s_fields[HS_LOCATN] = _r_fields[HR_URL];
                    // out (_s_fields[HS_LOCATN] + "<<")
                    if (server->_locations[i]._redirect.second == 301)
                        throw response_status(SC_301);
                    else
                        throw response_status(SC_302);
                }
                return (&server->_locations[i]);
            }
        }
        size_t pos = url.find_last_of("/");
        if (pos == 0 && url.length() > 1)
            pos++;
        if (pos == std::string::npos)
            pos = 0;
        url = url.substr(0, pos);
    }
    return (&server->_locations[0]);
}

void RecvHandler::set_servers(std::vector<ServerSet*>* _sv)
{
    _servers = _sv;
    _location = &get_matched_server(_servers)->_locations[0];
}
void RecvHandler::set_response(Response* _sv)
{
    _response = _sv;
}
bool RecvHandler::get_is_done(void)
{
    return (_is_done);
}
void RecvHandler::set_is_done(bool _sv)
{
    _is_done = _sv;
}

/******************************************************************************/
/*                                 fonctor                                    */
/******************************************************************************/

void RecvHandler::operator()(int fd, int ndata)
{
    if (_is_head)
        recv_head(fd, ndata);
    if (!_is_head)
        recv_body(fd, ndata);
}

/******************************************************************************/
/*                               recv request                                 */
/******************************************************************************/

void RecvHandler::recv_head(int fd, int &ndata)
{
    size_t  hif = _rlength;
    int     rdata = 0;
    size_t  dcrlf;
   
    do
    {
        if (_rlength == _rhead.length())
            _rhead.resize(_rlength * 2);
        _rlength += rdata = ::recv(fd, (void *)(_rhead.data() + _rlength), _rhead.size() - _rlength, 0);
        if (rdata == -1)
            throw server_error(std::string("error: recv: ") + ::strerror(errno));
        if (rdata == 0)
            throw server_error(std::string("Connection closed by client."));
        ndata -= rdata;
    } while (ndata > 0);
    dcrlf = find_dcrlf(_rhead, hif);
    if (dcrlf != std::string::npos)
    {
        try
        {
            RequestParser request_parser(&_rhead, &_r_fields);
            
            request_parser.parse_first_line();
            _location = get_matched_location(_servers);
            request_parser.parse_header();
            
            RequestProcessor request_processor(_response, &_r_fields, &_s_fields, _location);
            _is_head = _is_done = request_processor.process_request();
            
            if (_is_done)
                build_header(_response->_head, _s_fields);
            _rlength -= dcrlf;
            _rbody = &_rhead[dcrlf];
        }
        catch (const server_error &e)
        {
            std::cerr << e.what() << '\n';
            _s_fields[HS_STCODE] = SC_500;
            build_body(*_response, _s_fields, _location->_err_page);
            build_header(_response->_head, _s_fields);
            _is_done = true;
        }
        catch (const response_status &e)
        {
            _s_fields[HS_STCODE] = e.what();
            build_body(*_response, _s_fields, _location->_err_page);
            build_header(_response->_head, _s_fields);
            _is_done = true;
        }
        if (!_is_head && !_r_fields[HR_CNTLEN].empty())
            _rbody.resize(stoul(_r_fields[HR_CNTLEN]));
    }
}
void RecvHandler::recv_body(int fd, int ndata)
{
    int     rdata = 0;
    
    do
    {
        if (ndata > 0)
        {
            _rlength += rdata = ::recv(fd, (void *)(_rbody.data() + _rlength), _rbody.size() - _rlength, 0);
            if (rdata == -1)
                throw server_error(std::string("error: recv: ") + ::strerror(errno));
            if (rdata == 0)
                throw server_error(std::string("Connection closed by client."));
            ndata -= rdata;
        }
        if (_rlength == _rbody.length())
        {
            try
            {
                // out (_rbody)
                if (is_cgi(_r_fields, _location->_cgi))
                {
                    CGIExecutor cgi(_r_fields, *_location);

                    cgi.set_body(&_rbody);
                    cgi.execute_cgi();
                    mmap_file(*_response, "/tmp/cgi_tmp_file");
                    _s_fields[HS_LCRLF] = "";
                    _s_fields[HS_STCODE] = SC_201;
                    _s_fields[HS_CNTLEN] = std::to_string(_response->_body_len);
                }
                else
                {
                    UploadHandler uploader(&_rbody, &_r_fields);
                    _r_fields[HR_RURL] = _location->_upload_dir;
                    uploader.select_post_handler();
                }
                build_header(_response->_head, _s_fields);
                _is_done = true;
            }
            catch (const server_error &e)
            {
                std::cerr << e.what() << '\n';
                _s_fields[HS_STCODE] = SC_500;
                build_body(*_response, _s_fields, _location->_err_page);
                build_header(_response->_head, _s_fields);
                _is_done = true;
            }
            catch (const response_status &e)
            {
                _s_fields[HS_STCODE] = e.what();
                build_body(*_response, _s_fields, _location->_err_page);
                build_header(_response->_head, _s_fields);
                _is_done = true;
            }
        }
    } while (ndata > 0);
}
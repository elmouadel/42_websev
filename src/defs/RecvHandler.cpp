/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RecvHandler.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelha <eabdelha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/14 12:10:10 by eabdelha          #+#    #+#             */
/*   Updated: 2022/12/21 00:28:05 by eabdelha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../classes/RecvHandler.hpp"
#include "../../debug.h"

/******************************************************************************/
/*                            construct-destruct                              */
/******************************************************************************/

RecvHandler::RecvHandler() : _is_done(0), _is_head(1), _rlength(0)
{
    _rhead.resize(BUF_SIZE);
    _r_fields.resize(6);
    _s_fields.resize(5);
}
RecvHandler::~RecvHandler()
{
}

/******************************************************************************/
/*                              getters-setters                               */
/******************************************************************************/

void RecvHandler::set_servers(std::vector<ServerSet*>* _sv)
{
    _servers = _sv;
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
            RequestProcessor request_processor(_response, _servers, &_r_fields, &_s_fields);
            request_processor.set_err_page(_err_page);
            
            request_parser.parse_header();
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
            build_body(*_response, _s_fields, _err_page);
            build_header(_response->_head, _s_fields);
            _is_done = true;
        }
        catch (const response_status &e)
        {
            _s_fields[HS_STCODE] = e.what();
            build_body(*_response, _s_fields, _err_page);
            build_header(_response->_head, _s_fields);
            _is_done = true;
        }
        if (!_is_head)
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
                UploadHandler uploader(&_rbody, &_r_fields);
                uploader.select_post_handler();
            }
            catch (const server_error &e)
            {
                std::cerr << e.what() << '\n';
                _s_fields[HS_STCODE] = SC_500;
                build_body(*_response, _s_fields, _err_page);
                build_header(_response->_head, _s_fields);
                _is_done = true;
            }
            catch (const response_status &e)
            {
                _s_fields[HS_STCODE] = e.what();
                build_body(*_response, _s_fields, _err_page);
                build_header(_response->_head, _s_fields);
                _is_done = true;
            }
        }
    } while (ndata > 0);
}
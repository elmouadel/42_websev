/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIExecutor.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelha <eabdelha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/23 22:25:38 by eabdelha          #+#    #+#             */
/*   Updated: 2022/12/27 19:58:26 by eabdelha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../classes/CGIExecutor.hpp"
#include "../../ztrash/debug.h"

/******************************************************************************/
/*                            construct-destruct                              */
/******************************************************************************/

CGIExecutor::CGIExecutor(std::vector<std::string>& _r_fields, LocationSet& _location)
{
    char*       pwd;
    std::string ext;

    pwd = getcwd(NULL, 0);
    ext = _r_fields[HR_RURL].substr(_r_fields[HR_RURL].find_last_of(".") + 1, \
    _r_fields[HR_RURL].length() - _r_fields[HR_RURL].find_last_of(".") - 1);
    
    _env[ENV_CONTENT_TYPE] = std::string("CONTENT_TYPE=") + _r_fields[HR_CTYP].data();
    if (!_r_fields[HR_BONDRY].empty())
        _env[ENV_CONTENT_TYPE] += std::string(";") + "boundary=" + &_r_fields[HR_BONDRY][2];
    _env[ENV_CONTENT_LENGTH] = std::string("CONTENT_LENGTH=") + _r_fields[HR_CNTLEN].data();
    _env[ENV_PATH_INFO] = std::string("PATH_INFO=") + pwd + "/";
    _env[ENV_PATH_TRANSLATED] = std::string("PATH_TRANSLATED=") + pwd + "/";
    _env[ENV_QUERY_STRING] = std::string("QUERY_STRING=") + _r_fields[HR_QUERIES].data();
    _env[ENV_REQUEST_METHOD] = std::string("REQUEST_METHOD=") + _r_fields[HR_METHOD].data();
    _env[ENV_SCRIPT_FILENAME] = std::string("SCRIPT_FILENAME=") + _r_fields[HR_RURL];
    _env[ENV_SCRIPT_NAME] = std::string("SCRIPT_NAME=") + _r_fields[HR_RURL];
    // _env[ENV_SERVER_NAME] = std::string("SERVER_NAME=") + "127.0.0.1";
    // _env[ENV_SERVER_PORT] = std::string("SERVER_PORT=") + "8000";
    _env[ENV_SERVER_SOFTWARE] = std::string("SERVER_SOFTWARE=") + "nginy/1.33.7";
    _env[ENV_SERVER_PROTOCOL] = std::string("SERVER_PROTOCOL=") + "HTTP/1.1";
    _env[ENV_GATEWAY_INTERFACE] = std::string("GATEWAY_INTERFACE=") + "CGI/1.1";
    _env[ENV_REDIRECT_STATUS] = std::string("REDIRECT_STATUS=") + "0";

    if (_location._cgi[ext][0] != '/')
        _args[0] = std::string(pwd) + "/" + _location._cgi[ext]; 
    else
        _args[0] = _location._cgi[ext]; 
    _args[1] = _r_fields[HR_RURL];
    
    _body = nullptr;
}
CGIExecutor::~CGIExecutor()
{
}

void CGIExecutor::set_body(std::string *_sv)
{
    _body = _sv;
}
/******************************************************************************/
/*                                 execute cgi                                */
/******************************************************************************/
void CGIExecutor::execute_cgi(void)
{
    std::map<int, std::string>::iterator    it;
    const char*                             env[20];
    const char*                             args[3];
    int                                     status;
    int                                     pid;
    int                                     i;
    
    it = _env.begin();
    for (i = 0; it != _env.end(); ++it, ++i)
        env[i] = it->second.data();
    env[i] = nullptr;
    
    it = _args.begin();
    for (i = 0; it != _args.end(); ++it, ++i)
        args[i] = it->second.data();
    args[i] = nullptr;
    // out(*_body)
    int fds[2];
    pipe(fds);
    pid = fork();
    if (!pid)
    {
        for (size_t i = 0; i < _env.size(); ++i)
            out(env[i])
        for (size_t i = 0; i < _args.size(); ++i)
            out(args[i])
        
        int fd_out = open("/tmp/cgi_tmp_file", O_WRONLY | O_TRUNC | O_CREAT, S_IRWXU);
        dup2(fd_out, 1);
        close(fd_out);
        if (_body)
        {
            dup2(fds[0], 0);
        }
        execve(args[0], (char**)args, (char**)env);
        out (strerror(errno))
    }
    if (_body)
        write(fds[1], (*_body).data(), (*_body).length());
        close(fds[0]);
        close(fds[1]);
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
    {
        status = WEXITSTATUS(status);
        if (status != 0)
            throw response_status(SC_500);
    }
}
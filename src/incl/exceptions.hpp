/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelha <eabdelha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 17:34:06 by eabdelha          #+#    #+#             */
/*   Updated: 2023/01/11 13:42:14 by eabdelha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

# include <exception>
# include <sys/errno.h>

class failed_open : public std::exception
{
    public:
        const char *what() const throw()
        {
            return ("error: failed to open config file!.");
        }
};

class unclosed_block : public std::exception
{
    public:
        const char *what() const throw()
        {
            return ("error: unclosed block in config file!.");
        }
};

class not_srever_block : public std::exception
{
    public:
        const char *what() const throw()
        {
            return ("error: missing opening bracket in config file!.");
        }
};

class unexpected_token : public std::exception
{
    std::string msg;

    public:
        const char *what() const throw()
        {
            return (msg.data());
        }
        unexpected_token(std::string &_msg) : msg("error: unexpected token: " + _msg + "!.")
        {
        }
        ~unexpected_token() throw()
        {
        }
};

class err_token_args : public std::exception
{
    public:
        const char *what() const throw()
        {
            return ("error: invalid token args in config file!.");
        }
};

class err_strerror : public std::exception
{
    std::string msg;

    public:
        const char *what() const throw()
        {
            return (msg.data());
        }
        err_strerror(std::string _msg) : msg(_msg + ::strerror(errno))
        {
        }
        ~err_strerror() throw()
        {
        }
};

class response_status : public std::exception
{
    std::string msg;

    public:
        const char *what() const throw()
        {
            return (msg.data());
        }
        response_status(std::string _msg) : msg(_msg)
        {
        }
        ~response_status() throw()
        {
        }
};

class server_error : public std::exception
{
    std::string msg;

    public:
        const char *what() const throw()
        {
            return (msg.data());
        }
        server_error(std::string _msg) : msg(_msg)
        {
        }
        ~server_error() throw()
        {
        }
};

class return_unvalid : public std::exception
{
    public:
        const char *what() const throw()
        {
            return ("");
        }
};


#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MiniStructs.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelha <eabdelha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/14 14:14:44 by eabdelha          #+#    #+#             */
/*   Updated: 2022/12/16 10:25:44 by eabdelha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_STRUCTS_HPP
#define MINI_STRUCTS_HPP

# include <string>
# include "./macros.hpp"
# include <sys/mman.h>

struct Response
{
    std::string _head;
    char*       _body;
    size_t      _body_len;
    bool        _is_mapped;
    Response() : _body(nullptr), _body_len(0), _is_mapped(0) { _head.resize(BUF_SIZE) ;}
    ~Response() 
    {
        if (_is_mapped)
            munmap((void*)_body, _body_len);
    }
};


#endif
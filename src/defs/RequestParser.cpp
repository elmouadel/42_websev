/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelha <eabdelha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/14 16:59:26 by eabdelha          #+#    #+#             */
/*   Updated: 2022/12/21 08:57:15 by eabdelha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../classes/RequestParser.hpp"
#include "../../debug.h"

/******************************************************************************/
/*                          init static variables                             */
/******************************************************************************/
typedef void (*handler)(std::vector<std::string>&, const std::string&);
std::map<std::string, handler>  RequestParser::_assign_handl;
std::set<std::string>           RequestParser::_methods_list;

bool init_assign_handl(std::map<std::string, handler> &vmap)
{
    vmap["host"] = RequestParser::host_handle;
    vmap["content-type"] = RequestParser::ctyp_handle;
    vmap["content-length"] = RequestParser::clen_handle;
    vmap["transfer-encoding"] = RequestParser::tencod_handle;
    return (1);
}
bool init_methods_listl(std::set<std::string> &vset)
{
    vset.insert("GET");
    vset.insert("POST");
    vset.insert("DELETE");
    return (1);
}
bool dummy1 = init_assign_handl(RequestParser::_assign_handl);
bool dummy2 = init_methods_listl(RequestParser::_methods_list);
/******************************************************************************/
/*                            construct-destruct                              */
/******************************************************************************/
RequestParser::RequestParser(std::string* _cv1, std::vector<std::string>* _cv2)
    : _header(_cv1), _r_fields(_cv2)
{
}
RequestParser::~RequestParser()
{
}

/******************************************************************************/
/*                              getters-setters                               */
/******************************************************************************/

void RequestParser::set_header(std::string* _sv)
{
    _header = _sv;
}
void RequestParser::set_r_fields(std::vector<std::string>* _sv)
{
    _r_fields = _sv;   
}

/******************************************************************************/
/*                            assign handle methods                           */
/******************************************************************************/

void RequestParser::host_handle(std::vector<std::string> &fileds, const std::string &header)
{
    std::string word;

    get_first_word(header, word);
    if (is_empty(word))
        throw response_status(SC_400);
    fileds[HR_HOST] = word;
}

void RequestParser::tencod_handle(std::vector<std::string> &fileds, const std::string &header)
{
    std::string word;
    
    get_first_word(header, word);
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);
    if (word != "chunked")
        throw response_status(SC_415);
    fileds[HR_TENCOD] = word;
}

void RequestParser::clen_handle(std::vector<std::string> &fileds, const std::string &header)
{
    std::string word;
    
    get_first_word(header, word);
    if (word.find_first_not_of("0123456789") != std::string::npos || is_empty(word))
        throw response_status(SC_400);
    fileds[HR_CNTLEN] = word;
}

void RequestParser::ctyp_handle(std::vector<std::string> &fileds, const std::string &header)
{
    std::string word;
    size_t      pos;
    
    get_first_word(header, word);
    if (is_empty(word))
        throw response_status(SC_400);
    if (fileds[HR_METHOD] == "POST")
    {
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        if (word == "multipart/form-data")
        {
            pos = header.find("boundary=",  word.length());
            if (pos == std::string::npos)
                throw response_status(SC_415);
            pos += 9;
            fileds.push_back(header.substr(pos, header.find_first_of(" \r\n", pos) - pos));
            fileds[HR_BONDRY] = std::string("--") + fileds[HR_BONDRY];
            if (is_empty(fileds[HR_BONDRY]))
                throw response_status(SC_415);
        }
    }
    fileds[HR_CTYP] = word;
    // out(">>>>")
    // out(fileds[HR_CTYP])
    // out(fileds[HR_BONDRY])
    // out(">>>>")
}

/******************************************************************************/
/*                           header parsing functions                         */
/******************************************************************************/
void RequestParser::parse_first_line(void)
{
    std::vector<std::string> str_sq;
    get_sequence_str_v2(*_header, str_sq);
    if (str_sq.size() != 3)
        throw response_status(SC_400);
    if (_methods_list.find(str_sq[0]) == _methods_list.end())
        throw response_status(SC_405);
    if (str_sq[1].length() > URL_LIMIT)
        throw response_status(SC_414);
    if (str_sq[2] != "HTTP/1.1")
        throw response_status(SC_505);
    (*_r_fields)[HR_METHOD] = str_sq[0];
    (*_r_fields)[HR_URL] = str_sq[1];
}

void RequestParser::parse_header(void)
{
    parse_first_line();
    for (size_t i = 0; (*_header)[i]; ++i)
    {
        size_t p1 = _header->find_first_not_of(" ", i);
        size_t p2 = _header->find_first_of(":\r\n", p1);
        i = p2;
        // out(&(*_header)[i])
        if ((*_header)[p2] == ':')
        {
            std::map<std::string, handler>::iterator it;
            std::string str  = _header->substr(p1, p2 - p1);
            
            std::transform(str.begin(), str.end(), str.begin(), ::tolower);
            it = _assign_handl.find(str);
            if (it != _assign_handl.end())
                it->second(*_r_fields, std::string(&(*_header)[p2 + 1]));
            i = _header->find_first_of("\n", p2);
        }
    }
}
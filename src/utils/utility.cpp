/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelha <eabdelha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 15:36:25 by eabdelha          #+#    #+#             */
/*   Updated: 2022/12/20 22:52:38 by eabdelha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/utils.hpp"

inline bool is_space(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
}

bool is_comment(std::string &str)
{
    int i = 0;
    for (; is_space(str[i]); ++i)
    {
    }
    return (str[i] == '#');
}

bool is_empty(std::string &str)
{
    size_t i = 0;
    while (is_space(str[i]))
        i++;
    return (i == str.length());
}

size_t is_str_start_with(std::string &str, const std::string &tokens)
{
    size_t i = 0, j = 0;
    for (; i < str.length() && j < tokens.length(); ++i)
    {
        if (is_space(str[i]))
            continue;
        else if (str[i] != tokens[j])
            return (false);
        ++j;
    }
    if (j == tokens.length())
        return (i);
    return (false);
}

size_t get_first_word(const std::string &str, std::string &key_word)
{
    size_t fi, li;
    fi = str.find_first_not_of(" ");
    li = str.find_first_of(" ;\r\n", fi);
    
    key_word = str.substr(fi, li - fi);
    
    if (str[li] == ';')
        return (std::string::npos);
    return (li + 1);
}

void end_block(std::string &str)
{
    size_t rc = is_str_start_with(str, "}");
    if (rc)
        str = &str[rc];
    else
        throw unclosed_block();
}

void get_sequence_str(std::string &str, std::vector<std::string> &str_sq)
{
    std::string word;
    std::string str_hol;
    size_t rc;
    size_t rc_hol = 0;
    
    str_hol  = str.substr(0, str.find_first_of(';') + 1);
    
    rc_hol = str_hol.find_first_of(" ", str_hol.find_first_not_of(" "));
    while (1)
    {
        rc = get_first_word(&str_hol[rc_hol], word);
        if (!is_empty(word))
            str_sq.push_back(word);
        if (rc == std::string::npos)
            break;
        rc_hol += rc;
    }
}

void get_sequence_str_v2(std::string &str, std::vector<std::string> &str_sq)
{
    for (size_t i = 0; str[i] != '\n' && str[i] != '\r'; ++i)
    {
        str_sq.push_back(std::string());
        size_t _p1 = str.find_first_not_of(" ", i);
        size_t _p2 = str.find_first_of(" \r\n", _p1);
        str_sq.back() = str.substr(_p1, _p2 - _p1);
        i = _p2 - 1;
    }
}

void split_string(const std::string &str, std::vector<std::string> &str_sq, std::string charset)
{
    for (size_t i = 0; i < str.length(); ++i)
    {
        str_sq.push_back(std::string());
        size_t _p1 = str.find_first_not_of(" ", i);
        size_t _p2 = str.find_first_of(charset + "\"", _p1);
        if (str[_p2] == '"')
        {
            _p2 = str.find_first_of("\"", _p2 + 1);
            _p2 = str.find_first_of(charset, _p2);
        }
        str_sq.back() = str.substr(_p1, _p2 - _p1);
        i = _p2;
    }
}

size_t find_dcrlf(std::string &str, size_t pos)
{
    size_t _ret;

    if (pos > 4)
        pos -= 4;
    _ret = str.find("\r\n\r\n", pos);
    if (_ret != std::string::npos)
        return (str[_ret] = '\n', str[_ret + 1] = '\0', _ret + 4);
    _ret = str.find("\n\n", pos);
    if (_ret != std::string::npos)
        return (str[_ret] = '\n', str[_ret + 1] = '\0', _ret + 2);
    return (std::string::npos);
}

std::string	get_date(void)
{
    char    buf[100];
    time_t  now = time(0);
    tm      *_tm = localtime(&now);
    
    strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S %Z", _tm);
    return (std::string(buf));
}
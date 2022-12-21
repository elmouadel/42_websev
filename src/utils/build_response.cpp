/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_response.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelha <eabdelha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 21:42:10 by eabdelha          #+#    #+#             */
/*   Updated: 2022/12/21 00:38:40 by eabdelha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <map>
#include <string>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "../includes/utils.hpp"
#include "../includes/macros.hpp"
#include "../../debug.h"

void build_header(std::string &header, std::vector<std::string> &s_fields)
{
    header.assign("HTTP/1.1 ");
    header.append(s_fields[HS_STCODE]);
    header.append("\r\n");
    header.append("Server: nginy/1.33.7\r\n");
    header.append("Date: ");
    header.append(get_date());
    header.append("\r\n");
    if (!s_fields[HS_CNTLEN].empty())
    {
        header.append("Content-Length: ");
        header.append(s_fields[HS_CNTLEN]);
        header.append("\r\n");
    }
    if (!s_fields[HS_LOCATN].empty())
    {
        header.append("Location: ");
        header.append(s_fields[HS_LOCATN]);
        header.append("\r\n");
    }
    if (!s_fields[HS_ALLOWD].empty())
    {
        header.append("Allow: ");
        header.append(s_fields[HS_ALLOWD]);
        header.append("\r\n");
    }
    header.append("\r\n");
}

void build_body(Response &response, std::vector<std::string> &s_fields, std::map<int, std::string> err_page)
{
    std::string &file = err_page[stoi(s_fields[HS_STCODE].substr(0, 3))];
    int         fd;
    struct stat st;
    
    fd = open(file.data(), O_RDONLY);
    if (fd == -1) 
        throw server_error(std::string("error: open: ") + ::strerror(errno));
    if (fstat(fd, &st) < 0)
        throw server_error(std::string("error: fstat: ") + ::strerror(errno));
    response._body_len = st.st_size;
    if (!response._body_len)
        return;
    response._body = (char*)mmap(NULL, response._body_len, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    if (response._body == MAP_FAILED)
        throw server_error(std::string("error: mmap: ") + ::strerror(errno));
    response._is_mapped = true;
    s_fields[HS_CNTLEN] = std::to_string(response._body_len);
}
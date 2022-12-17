/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_response.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelha <eabdelha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 21:42:10 by eabdelha          #+#    #+#             */
/*   Updated: 2022/12/17 10:37:39 by eabdelha         ###   ########.fr       */
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

std::map<std::string, std::string> error_pages;

bool init_map_error_page(std::map<std::string, std::string> &vmap)
{
    vmap[SC_201] = "/Users/eabdelha/Desktop/v2_webserv/error_pages/ep_SC_201.html";
    vmap[SC_301] = "/Users/eabdelha/Desktop/v2_webserv/error_pages/ep_SC_301.html";
    vmap[SC_302] = "/Users/eabdelha/Desktop/v2_webserv/error_pages/ep_SC_302.html";
    vmap[SC_304] = "/Users/eabdelha/Desktop/v2_webserv/error_pages/ep_SC_304.html";
    vmap[SC_307] = "/Users/eabdelha/Desktop/v2_webserv/error_pages/ep_SC_307.html";
    vmap[SC_308] = "/Users/eabdelha/Desktop/v2_webserv/error_pages/ep_SC_308.html";
    vmap[SC_400] = "/Users/eabdelha/Desktop/v2_webserv/error_pages/ep_SC_400.html";
    vmap[SC_403] = "/Users/eabdelha/Desktop/v2_webserv/error_pages/ep_SC_403.html";
    vmap[SC_404] = "/Users/eabdelha/Desktop/v2_webserv/error_pages/ep_SC_404.html";
    vmap[SC_405] = "/Users/eabdelha/Desktop/v2_webserv/error_pages/ep_SC_405.html";
    vmap[SC_409] = "/Users/eabdelha/Desktop/v2_webserv/error_pages/ep_SC_409.html";
    vmap[SC_411] = "/Users/eabdelha/Desktop/v2_webserv/error_pages/ep_SC_411.html";
    vmap[SC_413] = "/Users/eabdelha/Desktop/v2_webserv/error_pages/ep_SC_413.html";
    vmap[SC_414] = "/Users/eabdelha/Desktop/v2_webserv/error_pages/ep_SC_414.html";
    vmap[SC_415] = "/Users/eabdelha/Desktop/v2_webserv/error_pages/ep_SC_415.html";
    vmap[SC_500] = "/Users/eabdelha/Desktop/v2_webserv/error_pages/ep_SC_500.html";
    vmap[SC_501] = "/Users/eabdelha/Desktop/v2_webserv/error_pages/ep_SC_501.html";
    vmap[SC_505] = "/Users/eabdelha/Desktop/v2_webserv/error_pages/ep_SC_505.html";
    return (false);
}

bool dummy3 = init_map_error_page(error_pages);

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

void build_body(Response &response, std::vector<std::string> &s_fields)
{
    std::string &file = error_pages[s_fields[HS_STCODE]];
    int         fd;
    struct stat st;
    
    fd = open(file.data(), O_RDONLY);
    if (fd == -1) 
        throw server_error(std::string("error: open: ") + ::strerror(errno));
    if (fstat(fd, &st) < 0)
        throw server_error(std::string("error: fstat: ") + ::strerror(errno));
    response._body_len = st.st_size;

    response._body = (char*)mmap(NULL, response._body_len, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    if (response._body == MAP_FAILED)
        throw server_error(std::string("error: mmap: ") + ::strerror(errno));
    response._is_mapped = true;
    s_fields[HS_CNTLEN] = std::to_string(response._body_len);
}
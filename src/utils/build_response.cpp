/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_response.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelha <eabdelha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 21:42:10 by eabdelha          #+#    #+#             */
/*   Updated: 2023/01/06 10:46:09 by eabdelha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <map>
#include <string>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "../incl/utils.hpp"
#include "../incl/macros.hpp"
#include "../../ztrash/debug.h"

std::map<std::string, std::string> mime_map;

bool init_mime_map(std::map<std::string, std::string> &mime_map)
{
        mime_map["html"] = "text/html";
        mime_map["htm"] = "text/html";
        mime_map["shtml"] = "text/html";
        mime_map["css"] = "text/css";
        mime_map["xml"] = "text/xml";
        mime_map["gif"] = "image/gif";
        mime_map["jpeg"] = "image/jpeg";
        mime_map["jpg"] = "image/jpeg";
        mime_map["js"] = "application/javascript";
        mime_map["atom"] = "application/atom+xml";
        mime_map["rss"] = "application/rss+xml";

        mime_map["mml"] = "text/mathml";
        mime_map["txt"] = "text/plain";
        mime_map["jad"] = "text/vnd.sun.j2me.app-descriptor";
        mime_map["wml"] = "text/vnd.wap.wml";
        mime_map["htc"] = "text/x-component";

        mime_map["png"] = "image/png";
        mime_map["svg"] = "image/svg+xml";
        mime_map["svgz"] = "image/svg+xml";
        mime_map["tif"] = "image/tiff";
        mime_map["tiff"] = "image/tiff";
        mime_map["wbmp"] = "image/vnd.wap.wbmp";
        mime_map["webp"] = "image/webp";
        mime_map["ico"] = "image/x-icon";
        mime_map["jng"] = "image/x-jng";
        mime_map["bmp"] = "image/x-ms-bmp";

        mime_map["woff"] = "font/woff";
        mime_map["woff2"] = "font/woff2";

        mime_map["jar"] = "application/java-archive";
        mime_map["war"] = "application/java-archive";
        mime_map["ear"] = "application/java-archive";
        mime_map["json"] = "application/json";
        mime_map["hqx"] = "application/mac-binhex40";
        mime_map["doc"] = "application/msword";
        mime_map["pdf"] = "application/pdf";
        mime_map["ps"] = "application/postscript";
        mime_map["eps"] = "application/postscript";
        mime_map["ai"] = "application/postscript";
        mime_map["rtf"] = "application/rtf";
        mime_map["m3u8"] = "application/vnd.apple.mpegurl";
        mime_map["kml"] = "application/vnd.google-earth.kml+xml";
        mime_map["kmz"] = "application/vnd.google-earth.kmz";
        mime_map["xls"] = "application/vnd.ms-excel";
        mime_map["eot"] = "application/vnd.ms-fontobject";
        mime_map["ppt"] = "application/vnd.ms-powerpoint";
        mime_map["odg"] = "application/vnd.oasis.opendocument.graphics";
        mime_map["odp"] = "application/vnd.oasis.opendocument.presentation";
        mime_map["ods"] = "application/vnd.oasis.opendocument.spreadsheet";
        mime_map["odt"] = "application/vnd.oasis.opendocument.text";

        mime_map["pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
        mime_map["xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
        mime_map["docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";

        mime_map["wmlc"] = "application/vnd.wap.wmlc";
        mime_map["7z"] = "application/x-7z-compressed";
        mime_map["cco"] = "application/x-cocoa";
        mime_map["jardiff"] = "application/x-java-archive-diff";
        mime_map["jnlp"] = "application/x-java-jnlp-file";
        mime_map["run"] = "application/x-makeself";
        mime_map["pl"] = "application/x-perl";
        mime_map["pm"] = "application/x-perl";
        mime_map["prc"] = "application/x-pilot";
        mime_map["pdb"] = "application/x-pilot";
        mime_map["rar"] = "application/x-rar-compressed";
        mime_map["rpm"] = "application/x-redhat-package-manager";
        mime_map["sea"] = "application/x-sea";
        mime_map["swf"] = "application/x-shockwave-flash";
        mime_map["sit"] = "application/x-stuffit";
        mime_map["tcl"] = "application/x-tcl";
        mime_map["tk"] = "application/x-tcl";
        mime_map["der"] = "application/x-x509-ca-cert";
        mime_map["pem"] = "application/x-x509-ca-cert";
        mime_map["crt"] = "application/x-x509-ca-cert";
        mime_map["xpi"] = "application/x-xpinstall";
        mime_map["xhtml"] = "application/xhtml+xml";
        mime_map["xspf"] = "application/xspf+xml";
        mime_map["zip"] = "application/zip";

        mime_map["bin"] = "application/octet-stream";
        mime_map["exe"] = "application/octet-stream";
        mime_map["dll"] = "application/octet-stream";
        mime_map["deb"] = "application/octet-stream";
        mime_map["dmg"] = "application/octet-stream";
        mime_map["iso"] = "application/octet-stream";
        mime_map["img"] = "application/octet-stream";
        mime_map["msi"] = "application/octet-stream";
        mime_map["msp"] = "application/octet-stream";
        mime_map["msm"] = "application/octet-stream";

        mime_map["mid"] = "audio/midi";
        mime_map["midi"] = "audio/midi";
        mime_map["kar"] = "audio/midi";
        mime_map["mp3"] = "audio/mpeg";
        mime_map["ogg"] = "audio/ogg";
        mime_map["m4a"] = "audio/x-m4a";
        mime_map["ra"] = "audio/x-realaudio";

        mime_map["3gpp"] = "video/3gpp";
        mime_map["3gp"] = "video/3gpp";
        mime_map["ts"] = "video/mp2t";
        mime_map["mp4"] = "video/mp4";
        mime_map["mpeg"] = "video/mpeg";
        mime_map["mpg"] = "video/mpeg";
        mime_map["mov"] = "video/quicktime";
        mime_map["webm"] = "video/webm";
        mime_map["flv"] = "video/x-flv";
        mime_map["m4v"] = "video/x-m4v";
        mime_map["mng"] = "video/x-mng";
        mime_map["asx"] = "video/x-ms-asf";
        mime_map["asf"] = "video/x-ms-asf";
        mime_map["wmv"] = "video/x-ms-wmv";
        mime_map["avi"] = "video/x-msvideo";
        return (1);
}

bool dummy3 = init_mime_map(mime_map);

std::string get_content_type(const char *file)
{
    std::string ext;

    ext = get_file_extention(std::string(file));
    if (!ext.empty())
    {
        std::map<std::string, std::string>::iterator it;

        it = mime_map.find(ext);
        if (it != mime_map.end())
            return (it->second);
    }
    return (std::string());
}

void build_header(std::string &header, std::vector<std::string> &s_fields)
{
    header.assign("HTTP/1.1 ");
    header.append(s_fields[HS_STCODE]);
    header.append("\r\n");
    header.append("Server: nginy/1.33.7\r\n");
    header.append("Date: ");
    header.append(get_date());
    header.append("\r\n");
    if (!s_fields[HS_CTYP].empty())
    {
        header.append("Content-Type: ");
        header.append(s_fields[HS_CTYP]);
        header.append("\r\n");
    }
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
    if (!s_fields[HS_CONNECT].empty())
    {
        header.append("Connection: ");
        header.append(s_fields[HS_CONNECT]);
        header.append("\r\n");
    }
    header.append(s_fields[HS_LCRLF]);
}

void build_body(Response &response, std::vector<std::string> &s_fields, std::map<int, std::string> err_page)
{
    std::string &file = err_page[stoi(s_fields[HS_STCODE].substr(0, 3))];
    
    mmap_file(response, file.data());
    s_fields[HS_CNTLEN] = std::to_string(response._body_len);
    s_fields[HS_CTYP] = get_content_type(file.data());
}

size_t get_pos_string(const char *str, size_t slen, const char *tof)
{
    for (size_t i = 0; i < slen; ++i)
    {
        size_t j = 0;
        size_t p = i;
        while (str[i++] == tof[j++])
        {
            if (!tof[j])
                return (p + j);
        }
        i = p;
    }
    return (std::string::npos);
}

void mmap_file(Response &response, const char *file)
{
    int         fd;
    struct stat st;
    
    fd = open(file, O_RDONLY);
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
}
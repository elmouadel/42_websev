/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UploadHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelha <eabdelha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/18 20:55:13 by eabdelha          #+#    #+#             */
/*   Updated: 2022/12/20 23:56:44 by eabdelha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../classes/UploadHandler.hpp"
#include "../../debug.h"

/******************************************************************************/
/*                            construct-destruct                              */
/******************************************************************************/

UploadHandler::UploadHandler(std::string *_cv1, std::vector<std::string>* _cv2) 
    : _body(_cv1), _r_fields(_cv2)
{
}

UploadHandler::~UploadHandler()
{
}
/******************************************************************************/
/*                             post body handlers                             */
/******************************************************************************/

void UploadHandler::select_post_handler(void)
{
    if ((*_r_fields)[HR_CTYP] == "multipart/form-data")
        upload_multipart_data();
}

void UploadHandler::upload_multipart_data(void)
{
    size_t      dcrlf;
    size_t      end;
    
    dcrlf = 0;
    end = 0;
    while (end != std::string::npos)
    {
        std::string filename;
        dcrlf = find_dcrlf(*_body, end);
        if (!parse_sub_header(_body->substr(end, dcrlf - end), filename))
            throw response_status(SC_201);
        end = write_data_to_file(filename, dcrlf);
    }
    throw response_status(SC_400);
}

bool UploadHandler::parse_sub_header(const std::string &sub_header, std::string &filename)
{
    std::string boundary;

    boundary = sub_header.substr(0, sub_header.find_first_of("\r\n"));
    if (sub_header.substr((*_r_fields)[HR_BONDRY].length(), 2) == "--")
        return (false);
    if (boundary != (*_r_fields)[HR_BONDRY])
        throw response_status(SC_400);
    for (size_t i = 0; sub_header[i]; ++i)
    {   
        size_t p1 = sub_header.find_first_not_of(" ", i);
        size_t p2 = sub_header.find_first_of(":\r\n", p1);
        i = p2;
        if (sub_header[p2] == ':')
        {
            std::string key_word  = sub_header.substr(p1, p2 - p1);
                
            std::transform(key_word.begin(), key_word.end(), key_word.begin(), ::tolower);
            if (key_word == "content-disposition")
                filename = content_disposition_handler(sub_header.substr(p2 + 1, sub_header.find_first_of("\r\n", p2) - p2));
            // else if (key_word == "content-type")
            //     content_type_handler(std::string(&sub_header[p2 + 1]));
            i = sub_header.find_first_of("\n", p2);
        }
    }
    return (true);
}

/******************************************************************************/
/*                               fields  handlers                             */
/******************************************************************************/
void UploadHandler::content_type_handler(const std::string& str)
{
    std::string word;

    get_first_word(str, word);
    if (word != "application/octet-stream")
        throw response_status(SC_415);
}
        
std::string UploadHandler::content_disposition_handler(const std::string& str)
{
    std::vector<std::string> sq_str;
    
    split_string(str, sq_str, " ;=\r\n");
    if (sq_str[0] != "form-data")
        throw response_status(SC_415);
    for (size_t i = 1; i < sq_str.size(); ++i)
    {
        std::transform(sq_str[i].begin(), sq_str[i].end(), sq_str[i].begin(), ::tolower);
        if (sq_str[i] == "filename" && i + 1 < sq_str.size())
        {
            if (sq_str[i + 1][0] == '\"' && sq_str[i + 1][sq_str[i + 1].length() - 1] == '\"')
                return (sq_str[i + 1].substr(1, sq_str[i + 1].length() - 2));
        }
    }
    return (std::string());
    
}

/******************************************************************************/
/*                                construct files                             */
/******************************************************************************/
size_t UploadHandler::write_data_to_file(std::string& filename, size_t start)
{
    filename = (*_r_fields)[HR_URL] + "/" + filename;
    
    std::ofstream   out_file(filename);
    size_t          end;
    
    if (!out_file.is_open())
       throw server_error(std::string(std::string("error: open: ") + ::strerror(errno)));
    end = _body->find((*_r_fields)[HR_BONDRY], start);
    if (end != std::string::npos)
        out_file.write((const char*)&(*_body)[start], end - start);
    return (end);
}
        
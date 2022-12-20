/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UploadHandler.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelha <eabdelha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/18 20:53:43 by eabdelha          #+#    #+#             */
/*   Updated: 2022/12/20 21:05:19 by eabdelha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UPLOAD_HANDLER_HPP
#define UPLOAD_HANDLER_HPP

# include <vector>
# include <string>
# include <fstream>
# include "../includes/utils.hpp"
# include "../includes/macros.hpp"

class UploadHandler
{
    std::string*                _body;
    std::vector<std::string>*   _r_fields;
    
    
    public:
        UploadHandler(std::string*, std::vector<std::string>*);
        ~UploadHandler();
        void select_post_handler(void);
        void upload_multipart_data(void);
        bool parse_sub_header(const std::string&, std::string&);
        void content_type_handler(const std::string&);
        std::string content_disposition_handler(const std::string&);
        size_t write_data_to_file(std::string&, size_t);
};

#endif
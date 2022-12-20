/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelha <eabdelha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/22 20:42:37 by eabdelha          #+#    #+#             */
/*   Updated: 2022/12/14 18:09:31 by eabdelha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

# include <map>
# include <ctime>
# include <string>
# include <vector>
# include <iomanip>
# include <iostream>
# include "./exceptions.hpp"
# include "../includes/MiniStructs.hpp"

bool        is_space(char c);
bool        is_comment(std::string &str);
bool        is_empty(std::string &str);
size_t      is_str_start_with(std::string &str, const std::string &tokens);
size_t      get_first_word(const std::string &str, std::string &key_word);
void        end_block(std::string &h_line);
void        get_sequence_str(std::string &str, std::vector<std::string> &str_sq);
void        get_sequence_str_v2(std::string &str, std::vector<std::string> &str_sq);
void        split_string(const std::string &str, std::vector<std::string> &str_sq, std::string charset);
size_t      find_dcrlf(std::string &str, size_t pos);
void        build_header(std::string &header, std::vector<std::string> &s_fields);
void        build_body(Response &response, std::vector<std::string> &s_fields, std::map<int, std::string> err_page);
std::string get_date(void);

#endif
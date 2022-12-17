/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSettings.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelha <eabdelha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/13 21:59:41 by eabdelha          #+#    #+#             */
/*   Updated: 2022/12/14 10:11:38 by eabdelha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_SETTINGS_HPP
#define SERVER_SETTINGS_HPP

# include <set>
# include <map>
# include <vector>
# include <string>


struct RedirectSet
{
    std::string _old_path;
    std::string _new_path;
    int         _err_code;
    RedirectSet() : _err_code(0) {}
    ~RedirectSet() {}
    bool operator==(RedirectSet &rhs)
    {
        return (_err_code == rhs._err_code && 
                _new_path == rhs._new_path && 
                _old_path == rhs._old_path);
    }
};

struct LocationSet
{
    std::map<int, std::string>  _err_page;
    std::vector<RedirectSet>    _redirect;
    std::set<std::string>       _acc_mtod;
    std::set<std::string>       _index;
    std::string                 _root;
    std::string                 _url_path;
    bool                        _autoindex;
    long                        _cb_max_size;
    LocationSet() : _autoindex(0), _cb_max_size(0) {}
    ~LocationSet() {}
};


struct ServerSet
{
    std::vector<LocationSet>    _locations;
    std::set<std::string>       _server_name;
    std::string                 _address;
    int                         _port;
    ServerSet() : _address("127.0.0.1"), _port(8080) {}
    ~ServerSet() {}
};

#endif
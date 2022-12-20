/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSettings.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelha <eabdelha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/13 21:59:41 by eabdelha          #+#    #+#             */
/*   Updated: 2022/12/21 00:10:28 by eabdelha         ###   ########.fr       */
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
    LocationSet() : _autoindex(0), _cb_max_size(0) 
    {
        _err_page[201] = "./error_pages/ep_SC_201.html";
        _err_page[301] = "./error_pages/ep_SC_301.html";
        _err_page[302] = "./error_pages/ep_SC_302.html";
        _err_page[304] = "./error_pages/ep_SC_304.html";
        _err_page[307] = "./error_pages/ep_SC_307.html";
        _err_page[308] = "./error_pages/ep_SC_308.html";
        _err_page[400] = "./error_pages/ep_SC_400.html";
        _err_page[403] = "./error_pages/ep_SC_403.html";
        _err_page[404] = "./error_pages/ep_SC_404.html";
        _err_page[405] = "./error_pages/ep_SC_405.html";
        _err_page[409] = "./error_pages/ep_SC_409.html";
        _err_page[411] = "./error_pages/ep_SC_411.html";
        _err_page[413] = "./error_pages/ep_SC_413.html";
        _err_page[414] = "./error_pages/ep_SC_414.html";
        _err_page[415] = "./error_pages/ep_SC_415.html";
        _err_page[500] = "./error_pages/ep_SC_500.html";
        _err_page[501] = "./error_pages/ep_SC_501.html";
        _err_page[505] = "./error_pages/ep_SC_505.html";
    }
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
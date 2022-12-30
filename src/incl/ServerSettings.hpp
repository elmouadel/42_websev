/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSettings.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelha <eabdelha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/13 21:59:41 by eabdelha          #+#    #+#             */
/*   Updated: 2022/12/28 17:32:12 by eabdelha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_SETTINGS_HPP
#define SERVER_SETTINGS_HPP

# include <set>
# include <map>
# include <vector>
# include <string>
# include <utility>

struct LocationSet
{
    std::map<std::string, std::string>  _cgi;
    std::map<int, std::string>          _err_page;
    std::pair<std::string, int>         _redirect;
    std::set<std::string>               _acc_mtod;
    std::set<std::string>               _index;
    std::string                         _root;
    std::string                         _url_path;
    std::string                         _upload_dir;
    size_t                              _cb_max_size;
    bool                                _autoindex;
    LocationSet() : _cb_max_size(1000000000), _autoindex(0)
    {
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
        _err_page[502] = "./error_pages/ep_SC_502.html";
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
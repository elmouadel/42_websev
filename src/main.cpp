/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eabdelha <eabdelha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/20 11:14:28 by eabdelha          #+#    #+#             */
/*   Updated: 2022/12/17 12:02:47 by eabdelha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <iostream>
#include "./includes/ServerSettings.hpp"
#include "./classes/ConfigParser.hpp"
#include "./classes/ServerLauncher.hpp"

int main(int ac, char **av)
{
    if (ac != 2)
    {
        std::cerr << "error: bad number of arguments!." << '\n';
        return (1);
    }
    try
    {
        std::vector<ServerSet>  server_set;
        {
            ConfigParser            config_parser(av[1], server_set);
            config_parser.main_config_file_parser();
            // config_parser.print();
        }
        ServerLauncher server_launcher(&server_set);
        server_launcher.launch_routines();
        server_launcher.core_server_loop();
        
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    
    
    return (0);    
}
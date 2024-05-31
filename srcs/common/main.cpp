/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdoukhan <jdoukhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 15:22:05 by aauthier          #+#    #+#             */
/*   Updated: 2024/05/31 15:17:05 by jdoukhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

int	main(int ac, char **av) {
	check_args(ac, av);
	std::cout << "HERE\n" << std::endl;
	Server server;
	server.init_server(ac, av);
	server.manage_loop();
	
	return (0);
}


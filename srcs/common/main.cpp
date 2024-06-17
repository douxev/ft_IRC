/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdoukhan <jdoukhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 15:22:05 by aauthier          #+#    #+#             */
/*   Updated: 2024/06/17 15:18:48 by jdoukhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "ft_irc.hpp"
# include <poll.h>
# include <signal.h>
# include <bits/sigaction.h>

static Server* get_server( Server* server ) {
	static Server* saved_server = NULL;

	if (saved_server && !server)
		return (saved_server);
	else
		saved_server = server;
	return NULL;
}

static void	sig_handler( int sig ) {
	(void) sig;
	std::cout << "\n" << SERVER_INFO << " SIG RECEIVED" << std::endl;
	get_server(NULL)->~Server();
	std::exit(0);
}

static void set_signals( void ) {
	struct sigaction	sa = {};

	sa.sa_handler = &sig_handler;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
}

int	main(int ac, char **av) {
	check_args(ac, av);

	set_signals();
	std::cout << SERVER_INFO << " Starting server" << std::endl;
	Server server;
	get_server(&server);
	server.init_server(ac, av);
	server.manage_loop();
	return (0);
}


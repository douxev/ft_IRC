/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdoukhan <jdoukhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 15:22:05 by aauthier          #+#    #+#             */
/*   Updated: 2024/05/31 15:58:11 by jdoukhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"
# include <poll.h>
# include <signal.h>
# include <bits/sigaction.h>

static void	sig_handler( int sig ) {
	(void) sig;
	std::cout << "SIG RECEIVED" << std::endl;
	exit(0);
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
	std::cout << "[Server] Starting server" << std::endl;
	Server server;
	server.init_server(ac, av);
	server.manage_loop();
	return (0);
}


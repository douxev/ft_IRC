/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdoukhan <jdoukhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 14:56:20 by jdoukhan          #+#    #+#             */
/*   Updated: 2024/05/30 14:21:07 by jdoukhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_HPP
# define FT_IRC_HPP


# include "Server.hpp"
# include "User.hpp"
# include "Channel.hpp"
#include <sys/poll.h>
#include <sys/socket.h> // For socket functions
#include <sys/types.h>	// For socket read/write
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib> // For exit(), EXIT_FAILURE and *alloc functions (poll() use array of struct)
#include <cerrno>	// For errno
#include <iostream> // For cout
#include <sstream>	// For stringstream
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include <ostream>
#include <stdio.h>

void	check_args(int ac, char **av);

void	ft_send(int socketfd, const void *c_str, size_t len);
short		get_port(int ac, char **av);

int		create_server_socket( short port );
void	accept_connection( int server_fd, struct pollfd **poll_fds, int& poll_count, int& poll_capacity );


#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdoukhan <jdoukhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 14:56:20 by jdoukhan          #+#    #+#             */
/*   Updated: 2024/06/19 13:52:09 by jdoukhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_HPP
# define FT_IRC_HPP


#include "Server.hpp"
#include "exceptions.hpp"
#include "User.hpp"
#include "Channel.hpp"
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

# define RESET "\033[0m"
# define YELLOW "\033[0;33m"
# define RED "\033[0;91m"
# define GREEN "\033[0;32m"
# define SERVER_INFO "\033[0;91m[SERVER]\033[0m "

void	check_args(int ac, char **av);




int		ft_send(int socketfd, std::string msg);
short	get_port(int ac, char **av);

int		create_server_socket( short port );
void	accept_connection( int server_fd, struct pollfd **poll_fds, int& poll_count, int& poll_capacity );
void	init_client( Server& server, int reply_socket, std::string message );

//Parsing commands
void	parse_commands( Server& server, int reply_socket, std::istringstream &message );
void	pong(int reply_socket, std::string message);
void	motd_command( Server& server, int reply_socket );
void	version_command( int reply_socket );
void	nick_command( Server& server, int reply_socket, std::string message );
void	cap_command( Server& server, int reply_socket, std::istringstream &message );
void	join_command( Server& server, int reply_socket, std::istringstream &message );
void	privmsg_command( Server& server, int reply_socket, std::istringstream &message );
void	mode_command( Server& server, int reply_socket, std::istringstream &message );
void	who_command( Server& server, int reply_socket, std::istringstream &message );
void	topic_command( Server& server, int reply_socket, std::istringstream &message );

void	names_command( Server& server, int reply_socket, std::istringstream &message );
void	names_command( Server& server, int reply_socket, std::string message );

void	list_command( Server& server, int reply_socket, std::istringstream &message );
void	invite_command( Server& server, int reply_socket, std::istringstream &message );
void	kick_command( Server& server, int reply_socket, std::istringstream &message );
void	part_command( Server& server, int reply_socket, std::istringstream &message );
void	quit_command( Server& server, int reply_socket, std::istringstream &message );
void	pass_command( Server& server, int reply_socket, std::istringstream &message );
void	whois_command( Server& server, int reply_socket, std::istringstream &message );
void	whois_command( Server& server, int reply_socket, std::string message );


void	no_topic_set( int reply_socket, std::string channel );

void	shutdown_command(Server& server);


#endif

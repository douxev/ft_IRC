/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rastie <rastie@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 15:22:05 by aauthier          #+#    #+#             */
/*   Updated: 2024/05/29 18:17:20 by rastie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include <sys/socket.h> // For socket functions
#include <sys/types.h>	// For socket read/write
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib> // For exit(), EXIT_FAILURE and *alloc functions (poll() use array of struct)
#include <cerrno>	// For errno
#include <iostream> // For cout
#include <sstream>	// For stringstream
#include "main.h"



int		create_server_socket( int port ) {
	struct sockaddr_in sa;
	    
	memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET; // IPv4
    sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // 127.0.0.1, localhost
    sa.sin_port = htons(port);

	int socket_fd = socket(sa.sin_family, SOCK_STREAM, 0);
	if (socket_fd == -1) {
		std::cerr << "[Server] Socket error: " << strerror(errno) << std::endl;
		return (-1);
	}
	std::cout << "[Server] Created server socket fd: " << socket_fd << std::endl;
	
	if (bind(socket_fd, &sa, sizeof(sa)))
	{
		std::cerr << "[Server] Bind error: " <<  strerror(errno) << std::endl;
        return (-1);
	}
	std::cout << "[Server] Bound server socket to port: " << port << std::endl;

	return(socket_fd);
}

void	accept_connection( int server_fd, struct pollfd **poll_fds, int& poll_count, int& poll_capacity ) {
	int client_fd;
	std::stringstream msg_to_sent;
	
	client_fd = accept(server_fd, NULL, NULL);
	if (client_fd == -1) {
		std::cerr << "[Server] Accept error: " <<  strerror(errno) << std::endl;
        return ;
	}
	
	if (poll_count == poll_capacity)
	{
		poll_capacity *= 2;
		*poll_fds = realloc(*poll_fds, poll_capacity * sizeof(**poll_fds));
	}
	*poll_fds[poll_count].fd = client_fd;
	*poll_fds[poll_count].events = POLLIN;
	poll_count++;
	std::cout << "[Server] Accepted connectinon from client " << client_fd << std::endl;
	
	msg_to_sent << "[Server] Welcome client " << client_fd << std::endl;
	if (send(client_fd, msg_to_sent.str().c_str(), msg_to_sent.str().size()), 0) == -1)
		std::cerr << "[Server] Send error to client " << client_fd << ": " <<  strerror(errno)  << std::endl;
	//TODO: Ajouter le client dans la liste des clients connectes;
	/*
	User new = new User();
	std::stringstream name;
	name << "Client" << client_fd;
	new.set_name(name.str());
	new.set_socket_fd(client_fd);
	_connected_users.push_back(new);
	*/
}

void	read_socket_data(int i, struct pollfd *pollfd, int& poll_count, int server_fd) {
	char buffer[BUFSIZ] = {0};
	int byte_read;
	int sender_fd;
	std::stringstream msg_to_sent;
	
	sender_fd = pollfd[i].fd;
	byte_read = recv(sender_fd, buffer, BUFSIZ, 0);
	if (byte_read <= 0) {
		if (!byte_read)
			std::cout << "[Server] Connection closed with client " << sender_fd << std::endl;
		else
			std::cerr << "[Server] Recv error: " << strerror(errno) << std::endl;
		close(sender_fd);
		pollfd[i] = pollfd[poll_count - 1];
		poll_count--;
		//TODO: enlever le client de la liste des clients connectes
	} else {
		std::cout << "[Server] Got message from client " << sender_fd << ": " << buffer << std::endl;
		if (buffer[0] == '/')
			//command
			;
		else
		{
			//send to all other clients
			msg_to_sent << "Client [" << sender_fd << "] said: " << buffer << std::endl;
			for (int j = 0; j < poll_count; j++)
			{
				if (pollfd[j].fd != server_fd && pollfd[j].fd != sender_fd)
					if (send(pollfd[j].fd, msg_to_sent.str().c_str(), msg_to_sent.str().size(), 0) == -1)
							std::cerr << "[Server] Send error to client " << pollfd[j].fd << ": " <<  strerror(errno)  << std::endl;
			}
		}
		
	}
	
}
int	main(void) {

	struct pollfd *poll_fds;
	int server_socket_fd;
	int poll_size;
	int poll_count;
	std::cout << "HERE\n" << std::endl;
	server_socket_fd = create_server_socket(PORT);
	
	if (server_socket_fd == -1)
		return (1);
	
	printf("[Server] Listening on port %d\n", PORT);
    if (listen(server_socket, 10)) {
		std::cerr << "[Server] Listen error: " <<  strerror(errno) << std::endl;
        return (2);
	}
	
	poll_size = 2;
	poll_fds = calloc(poll_size + 1, sizeof(*poll_fds));
	if (!poll_fds)
		return (3);
	
	poll_fds[0].fd = server_socket_fd;
	poll_fds[0].events = POLLIN;
	poll_count = 1;


	while (true)
	{
		int status = poll(poll_fds, poll_count, 2000);
		if (status == -1) {
			std::cerr << "[Server] Poll error: " <<  strerror(errno) << std::endl;
			free(poll_fds);
        	return (4);
		}
		if (!status){
			std::cout << "[Server] Waiting for connnection...\n";
			continue ;
		}
		
		for (int i = 0; i < poll_count; i++)
		{
			if ((poll_fds[i].revents & POLLIN) =! -1)
				continue;
			if (poll_fds[i].fd == server_socket_fd) {
				accept_connection(i, poll_fds, poll_count, poll_size);
			} else {
				read_socket_data(i, poll_fds, poll_count, server_socket_fd);
			}
		}
		
	}
	
	return (0);
}



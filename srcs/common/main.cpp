/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rastie <rastie@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 15:22:05 by aauthier          #+#    #+#             */
/*   Updated: 2024/05/30 16:23:18 by rastie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

void	read_socket_data(int i, struct pollfd *pollfd, int& poll_count, int server_fd) {
	char buffer[BUFSIZ] = {0};
	std::stringstream msg_to_sent;
	const int sender_fd = pollfd[i].fd;
	const int byte_read = recv(sender_fd, buffer, BUFSIZ, 0);
	
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
		std::cout << "[Server] Got message from client " << sender_fd << ": " << buffer;
		if (buffer[0] == '/')
			//command
			;
		else
		{
			//send to all other clients
			msg_to_sent << "Client [" << sender_fd << "] said: " << buffer;
			for (int j = 0; j < poll_count; j++)
			{
				// send(pollfd[j].fd, "PING TEstitesto", 16 , 0);
				if (pollfd[j].fd != server_fd)
					if (send(pollfd[j].fd, (msg_to_sent.str()).c_str(), msg_to_sent.str().size(), 0) == -1)
							std::cerr << "[Server] Send error to client " << pollfd[j].fd << ": " <<  strerror(errno)  << std::endl;
			}
		}
		
	}
	
}


int	main(int ac, char **av) {

	struct pollfd	*poll_fds;
	int				server_socket_fd;
	int				poll_size;
	int				poll_count;

	check_args(ac, av);
	std::cout << "HERE\n" << std::endl;
	Server server;
	server.init_server(ac, av);
	server.manage_loop();
	
	return (0);
}



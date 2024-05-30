/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdoukhan <jdoukhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 15:22:05 by aauthier          #+#    #+#             */
/*   Updated: 2024/05/30 14:59:17 by jdoukhan         ###   ########.fr       */
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
	server_socket_fd = create_server_socket(get_port(ac, av));
	
	if (server_socket_fd == -1)
		return (1);
	
	printf("[Server] Listening on port %d\n", get_port(ac, av));
    if (listen(server_socket_fd, 10)) {
		std::cerr << "[Server] Listen error: " <<  strerror(errno) << std::endl;
        return (2);
	}
	
	poll_size = 2;
	poll_fds = static_cast<struct pollfd *>(calloc(poll_size + 1, sizeof(*poll_fds)));
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
			// std::cout << "[Server] Waiting for connnection...\n";
			continue ;
		}
		
		for (int i = 0; i < poll_count; i++)
		{
			if ((poll_fds[i].revents & POLLIN) != 1)
				continue;
			if (poll_fds[i].fd == server_socket_fd) {
				accept_connection(server_socket_fd, &poll_fds, poll_count, poll_size);
			} else {
				read_socket_data(i, poll_fds, poll_count, server_socket_fd);
			}
		}
		
	}
	
	return (0);
}



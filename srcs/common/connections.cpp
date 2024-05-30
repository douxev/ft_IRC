#include "ft_irc.hpp"

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
		*poll_fds = static_cast<struct pollfd *>(realloc((*poll_fds), poll_capacity * sizeof(**poll_fds)));
	}
	poll_fds[poll_count]->fd = client_fd;
	poll_fds[poll_count]->events = POLLIN;
	poll_count++;
	std::cout << "[Server] Accepted connectinon from client " << client_fd << std::endl;
	
	msg_to_sent << "[Server] Welcome client " << client_fd << std::endl;
	if (send(client_fd, msg_to_sent.str().c_str(), msg_to_sent.str().size(), 0) == -1)
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
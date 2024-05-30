#include "Server.hpp"
#include <iostream>

Server::Server( void ) {}

Server& Server::operator=( const Server& Other ) {

	this->_ip_str = Other._ip_str;
	this->_ip_address = Other._ip_address;
	this->_port = Other._port;
	this->_server_socket = Other._server_socket;
	this->_active_channel = Other._active_channel;
	this->_connected_users = Other._connected_users;
}

Server::Server( const Server& copied ) {

	*this = copied;
}

Server::~Server() {}

void Server::_add_active_channel( const Channel& channel ) {
	this->_active_channel.push_back(channel);
}

void Server::_remove_unactive_channel( const Channel& channel ) {
	const size_t len = this->_active_channel.size();
	for (size_t i = 0; i < len; i++) {
		if (this->_active_channel[i] == channel) {
			this->_active_channel.erase(this->_active_channel.begin() + i);
			break;
		}
	}
}

void Server::_welcome_message() {

	std::cout<<"Welcome to GuiRaMa/n list of commands : etc"<<std::endl;
}

int Server::_init_server(int ac, char **av)
{
	struct pollfd	*poll_fds;

	_server_socket = create_server_socket(get_port(ac, av));
	if (_server_socket == -1)
		return (1);
	
	if (listen(_server_socket, 10)) {
		std::cerr << "[Server] Listen error: " <<  strerror(errno) << std::endl;
        return (2);
	}
	std::cout << "[Server] Listening on port " << _server_socket << std::endl;

	_sockets_fds[0].fd = _server_socket;
	_sockets_fds[0].events = POLLIN;
	_nb_sockets = 1;
}

void Server::_manage_loop()
{
	while (true)
	{
		int status = poll(_sockets_fds.data(), _nb_sockets, 2000);
		if (status == -1){
			std::cerr << "[Server] Poll error: " <<  strerror(errno) << std::endl;
			exit (4) ; //throw exception
		}
		if (!status)
			// std::cout << "[Server] Waiting for connnection...\n";
			continue ;

		for (int i = 0; i < _nb_sockets; i++)
		{
			if ((_sockets_fds[i].revents & POLLIN) != 1)
				continue;
			if (_sockets_fds[i].fd == _server_socket)
				_accept_connection();
			else
				_read_data(i);

			
		}
	}

}

void Server::_accept_connection()
{
	const int client_fd = accept(_server_socket, NULL, NULL);
	std::stringstream msg_to_sent;
	std::stringstream name;
	
	if (client_fd == -1) {
		std::cerr << "[Server] Accept error: " <<  strerror(errno) << std::endl;
        return ;
	}
	std::cout << "[Server] Accepted connection from client " << client_fd << std::endl;
	
	_sockets_fds[_nb_sockets].fd = client_fd;
	_sockets_fds[_nb_sockets].events = POLLIN;
	_nb_sockets++;
	
	name << "Client" << client_fd;
	_connected_users[client_fd] = name.str();
	
	msg_to_sent << "1 [Server] Welcome client " << client_fd << std::endl;
	if (send(client_fd, msg_to_sent.str().c_str(), msg_to_sent.str().size(), 0) == -1)
		std::cerr << "[Server] Send error to client " << client_fd << ": " <<  strerror(errno)  << std::endl;
	
}

void Server::_read_data(int i)
{
	char buffer[BUFSIZ] = {0};
	std::stringstream msg_to_sent;
	const int sender_fd = _sockets_fds[i].fd;
	const int byte_read = recv(sender_fd, buffer, BUFSIZ, 0);
	if (byte_read <= 0) {
		if (!byte_read)
			std::cout << "[Server] Connection closed with client " << sender_fd << std::endl;
		else
			std::cerr << "[Server] Recv error: " << strerror(errno) << std::endl;
		close(sender_fd);
		_sockets_fds[i] = _sockets_fds[_nb_sockets - 1];
		_nb_sockets--;
		_connected_users[sender_fd] = "";
		//TODO: enlever le client de la liste des clients connectes
	}else
	{
		std::cout << "[Server] Got message from client " << sender_fd << ": " << buffer;
		//Parsing
		msg_to_sent << "Client [" << sender_fd << "] said: " << buffer;
			for (int j = 0; j < _nb_sockets; j++)
			{
				// send(pollfd[j].fd, "PING TEstitesto", 16 , 0);
				if (_sockets_fds[j].fd != _server_socket)
					if (send(_sockets_fds[j].fd, (msg_to_sent.str()).c_str(), msg_to_sent.str().size(), 0) == -1)
							std::cerr << "[Server] Send error to client " << _sockets_fds[j].fd << ": " << strerror(errno) << std::endl;
			}
	}
	
}

#include "Server.hpp"
#include "error_numeric.hpp"
#include "ft_irc.hpp"
#include "exceptions.hpp"
#include <cstddef>
#include <iostream>
#include <sstream>
#include <sys/poll.h>

Server::Server( void ): 
_ip_address(0), _port(0), _server_socket(0), _nb_sockets(0) {}

Server::~Server() {
	const size_t users_len = this->_connected_users.size();
	const size_t chans_len = this->_active_channels.size();

	std::cout << "Server is being detroyed." << std::endl;

	for (size_t i = 0; i < users_len; i++) {
		delete this->_connected_users[i];
	}
	for (size_t i = 0; i < chans_len; i++) {
		delete this->_active_channels[i];
	}
}

Server& Server::operator=( const Server& Other ) {

	this->_ip_str = Other._ip_str;
	this->_ip_address = Other._ip_address;
	this->_port = Other._port;
	this->_server_socket = Other._server_socket;
	this->_active_channels = Other._active_channels;
	this->_connected_users = Other._connected_users;
	return (*this);
}

Server::Server( const Server& copied ) {

	*this = copied;
}


void Server::_add_active_channel( Channel* channel ) {
	this->_active_channels.push_back(channel);
}


void Server::_remove_unactive_channel( const Channel *channel ) {
	const size_t len = this->_active_channels.size();
	for (size_t i = 0; i < len; i++) {
		if (this->_active_channels[i] == channel) {
			this->_active_channels.erase(this->_active_channels.begin() + i);
			break;
		}
	}
}

std::string Server::get_motd( void ) {
	return (this->_motd);
}

bool	Server::nick_already_taken( std::string name ) const {
	const size_t len = this->_connected_users.size();
	for (size_t i = 0; i < len; i++) {
		if (_connected_users[i]->get_name() == name)
			return (true);
	}
	return (false);
}

void	Server::change_nick( User& user, std::string name ) {
	if (nick_already_taken(name))
		throw NickAlreadyTakenException();
	user.set_name(name);
}

User&	Server::get_user_class( std::string name ) {
	const size_t len = this->_connected_users.size();

	for (size_t i = 0; i < len; i++) {
		if (this->_connected_users[i]->get_name() == name)
			return (*this->_connected_users[i]);
	}
	throw NoSuchNickException();
}

Channel&	Server::get_channel_class( std::string name ) {
	const size_t len = this->_active_channels.size();

	for (size_t i = 0; i < len; i++) {
		if (this->_active_channels[i]->get_name() == name)
			return (*this->_active_channels[i]);
	}
	throw NoSuchChannelException();
}

std::vector<Channel *> Server::get_channels_list()
{
    return this->_active_channels;
}

std::vector<User *> Server::get_connected_user()
{
    return _connected_users;
}

int Server::init_server(int ac, char **av) {
	_server_socket = create_server_socket(get_port(ac, av));
	if (_server_socket == -1)
		return (1);
	
	if (listen(_server_socket, 10)) {
		std::cerr << "[Server] Listen error: " <<  strerror(errno) << std::endl;
        return (2);
	}
	std::cout << "[Server] Listening on port " << _server_socket << std::endl;

	struct pollfd new_poll_fd = {};
	this->_sockets_fds.push_back((new_poll_fd));
	_sockets_fds[0].fd = _server_socket;
	_sockets_fds[0].events = POLLIN;
	_sockets_fds[0].revents = 0;
	_nb_sockets = 1;
	return (0);
}

void Server::manage_loop()
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
			if (_sockets_fds[i].fd == _server_socket) {
				_accept_connection();
			}
			else {
				_read_data(i);

			}

			
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
	

	struct pollfd new_poll_fd = {};
	this->_sockets_fds.push_back((new_poll_fd));
	_sockets_fds[_nb_sockets].fd = client_fd;
	_sockets_fds[_nb_sockets].events = POLLIN;
	_sockets_fds[_nb_sockets].revents = 0;
	_nb_sockets++;
	
	name << "Client" << client_fd;
	User *client = new User();
	client->set_name(name.str());
	client->set_fd(client_fd);
	_connected_users.push_back(client);
	
	
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
		try {
			//user_quit(client);
			std::vector<User*>::iterator it = find(_connected_users.begin(), _connected_users.end(), 
													&get_user_class(sender_fd));
			_connected_users.erase(it);
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << "\n";
		}
	} else {
		std::cout << "[" << sender_fd << "] sent: " << buffer;
		// //Parsing
		std::istringstream stream(buffer);
		parse_commands(*this, sender_fd, stream);
		// msg_to_sent << "Client [" << sender_fd << "] said: " << buffer;
		// for (int j = 0; j < _nb_sockets; j++) {
		// 	// send(pollfd[j].fd, "PING TEstitesto", 16 , 0);
		// 	if (_sockets_fds[j].fd != _server_socket)
		// 		if (send(_sockets_fds[j].fd, (msg_to_sent.str()).c_str(), msg_to_sent.str().size(), 0) == -1)
		// 				std::cerr << "[Server] Send error to client " << _sockets_fds[j].fd << ": " << strerror(errno) << std::endl;
		// }
	}
}
User&	Server::get_user_class( int socketfd ) {
	const size_t len = this->_connected_users.size();

	for (size_t i = 0; i < len; i++) {
		if (this->_connected_users[i]->get_socketfd() == socketfd)
			return (*this->_connected_users[i]);
	}
	throw UserNotFoundException();
}

void	Server::join_channel( std::string username, std::string channelname ) {
	User&		user = this->get_user_class(username);

	try {
		Channel&	channel = this->get_channel_class(channelname);
		channel.user_join(user);
	}
	catch (NoSuchChannelException& e) {
		Channel *new_channel = new Channel(channelname, user);
		this->_active_channels.push_back(new_channel);
		new_channel->force_op(user);
		std::cout << "Created channel #" << this->get_channel_class(channelname).get_name() << std::endl;
		return ;
	}
}

void	Server::part_channel( std::string username, std::string channelname, std::string part_message ) {
	Channel&	channel = this->get_channel_class(channelname);
	User&		user = this->get_user_class(username);

	channel.user_quit(user, part_message);
	
}

bool	Server::is_on_channel( std::string channel, std::string user ) {	
	return this->get_channel_class(channel)
		.is_on_channel(this->get_user_class(user).get_name());
}

bool	Server::is_op( std::string channel, std::string user ) {
	return this->get_channel_class(channel)
		.is_op(this->get_user_class(user).get_name());
} 

std::string Server::get_topic( std::string channel ) {
	return this->get_channel_class(channel).get_topic();
}

void Server::set_topic( std::string channelname, std::string topic, std::string topic_whotime ) {

		Channel &channel = this->get_channel_class(channelname);
		channel.set_topic(topic, topic_whotime);
}
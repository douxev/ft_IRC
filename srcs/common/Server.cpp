#include "Server.hpp"
#include "error_numeric.hpp"
#include "ft_irc.hpp"
#include "exceptions.hpp"
#include <cctype>
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

	// this->_connected_users.clear();
	// this->_active_channels.clear();
	// this->_sockets_fds.clear();
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

void Server::remove_poll_fd( int fd )
{
	for (int i = 0; i < _nb_sockets; i++)
	{
		if (_sockets_fds[i].fd == fd) {
			close(fd);
			_sockets_fds[i] = _sockets_fds[_nb_sockets - 1];
			_nb_sockets--;
			break ;
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

std::vector<Channel *>& Server::get_channels_list()
{
    return this->_active_channels;
}

std::vector<User *>& Server::get_connected_user()
{
    return _connected_users;
}

int Server::init_server(int ac, char **av) {
	_server_socket = create_server_socket(get_port(ac, av));
	if (av[2])
		_password = av[2];
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

std::string Server::get_ip( void ) {
	return (this->_ip_str);
}

std::string Server::get_pass(void)
{
    return _password;
}

void Server::_read_data(int i)
{
	char buffer[BUFSIZ] = {0};
	std::stringstream msg_to_sent;
	const int sender_fd = _sockets_fds[i].fd;
	const int byte_read = recv(sender_fd, buffer, BUFSIZ, 0);
	if (byte_read <= 0) {
		if (!byte_read) {
			std::cout << "[Server] Connection closed with client " << sender_fd << std::endl;
		}
		else
			std::cerr << "[Server] Recv error: " << strerror(errno) << std::endl;
		close(sender_fd);
		_sockets_fds[i] = _sockets_fds[_nb_sockets - 1];
		_nb_sockets--;
	} else {
		std::cout << "[RECV" << sender_fd << "] " << buffer;
		std::istringstream stream(buffer);
		parse_commands(*this, sender_fd, stream);
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

void	Server::join_channel( std::string username, std::string channelname, std::string password ) {
	User&		user = this->get_user_class(username);

	try {
		Channel&	channel = this->get_channel_class(channelname);
		channel.user_join(user, password);
	}
	catch (NoSuchChannelException& e) {
		Channel *new_channel = new Channel(channelname, user);
		this->_active_channels.push_back(new_channel);
		std::cout << "Created channel " << this->get_channel_class(channelname).get_name() << std::endl;
		return ;
	}
}

void	Server::part_channel( std::string username, std::string channelname, std::string part_message ) {
	Channel&	channel = this->get_channel_class(channelname);
	User&		user = this->get_user_class(username);

	if (part_message.empty())
		part_message = username + " left the channel";
	channel.user_part(user, part_message);
}

bool	Server::is_on_channel( std::string channel, std::string user ) {	
	return this->get_channel_class(channel)
		.is_on_channel(this->get_user_class(user).get_name());
}

bool	Server::is_op( std::string channel, std::string user ) {
	return this->get_channel_class(channel)
		.is_op(this->get_user_class(user).get_name());
} 

bool	Server::channel_exists( std::string channel ) {
	try {
		this->get_channel_class(channel);
	}
	catch (...) {
		return (false);
	}
	return (true);
}

std::string Server::get_topic( std::string channel ) {
	return this->get_channel_class(channel).get_topic();
}

void	Server::send_all( std::string msg ) {
	const size_t len = this->_connected_users.size();

	for (size_t i = 0; i < len; i++) {
		ft_send(this->_connected_users[i]->get_socketfd(), msg);
	}
}

void	Server::send_all( int reply_socket, std::string msg ) {
	const size_t len = this->_connected_users.size();

	for (size_t i = 0; i < len; i++) {
		if (!(this->_connected_users[i]->get_socketfd() == reply_socket))
			ft_send(this->_connected_users[i]->get_socketfd(), msg);
	}
}


void Server::set_topic( std::string channelname, std::string topic, std::string topic_whotime ) {

		Channel &channel = this->get_channel_class(channelname);
		channel.set_topic(topic, topic_whotime);
}

void Server::send_channel( std::string channelname, int sender_fd, std::string msg ) {

	Channel &channel = this->get_channel_class(channelname);
	channel.send_channel(sender_fd, msg);
}

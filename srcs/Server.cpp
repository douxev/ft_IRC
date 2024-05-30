#include "Server.hpp"
#include <iostream>

Server::Server( void ) {}

Server& Server::operator=( const Server& Other ) {

	this->_ip_str = Other._ip_str;
	this->_ip_address = Other._ip_address;
	this->_port = Other._port;
	this->_socket = Other._socket;
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
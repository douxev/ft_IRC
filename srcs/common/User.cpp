#include "User.hpp"
#include <algorithm>
#include <iostream>

User::User( void ) {
	this->_name = "";
	this->_socketfd = 0;
	this->_ip_address = "";
}

User::User( const User& Other ) {
	this->_name = Other._name;
	this->_socketfd = Other._socketfd;
	this->_ip_address = Other._ip_address;
}

User& User::operator=( const User& Other ) {
	this->_name = Other._name;
	this->_socketfd = Other._socketfd;
	this->_ip_address = Other._ip_address;
	return (*this);
}

bool User::operator==( const User& Other ) const {
	if (this->_name == Other._name)
		return (true);
	return (false);
}


User::~User() {

}

void User::set_name( std::string name ) {
	this->_name = name;
}

void User::set_realname( std::string name ) {
	this->_realname = name;
}

void User::set_fd(int fd)
{
	_socketfd = fd;
}

const std::string User::get_name( void ) const {
	return (this->_name);
}

int			User::get_socketfd( void ) const {
	return (this->_socketfd);
}

std::vector<Channel *> User::get_list_channel()
{
    return _joined_channel;
}

void User::add_channel_list(Channel *channel)
{
	_joined_channel.push_back(channel);
}

void User::remove_channel_list(Channel *channel)
{
	std::vector<Channel*>::iterator it = find(this->_joined_channel.begin(), _joined_channel.end(), channel);
	_joined_channel.erase(it);
}

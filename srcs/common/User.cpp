#include "User.hpp"
#include <algorithm>
#include <cstddef>

User::User( void ) {
	this->_name = "";
	this->_socketfd = 0;
	this->_ip_address = "127.0.0.1";
	_password_passed = false;
}

User::User( const User& Other ) {
	this->_name = Other._name;
	this->_socketfd = Other._socketfd;
	this->_ip_address = Other._ip_address;
	_password_passed = Other._password_passed;

}

User& User::operator=( const User& Other ) {
	this->_name = Other._name;
	this->_socketfd = Other._socketfd;
	this->_ip_address = Other._ip_address;
	_password_passed = Other._password_passed;

	return (*this);
}

bool User::operator==( const User& Other ) const {
	if (this->_name == Other._name)
		return (true);
	return (false);
}

void	User::_change_op_nick( const std::string new_name ) {
	std::vector<Channel *>& chans = this->get_list_channel();
	const size_t len = chans.size();

	for (size_t i = 0; i < len; i++) {
		chans[i]->change_op_nick(this->get_name(), new_name);
	}
}

User::~User() {

}

void User::set_name( std::string name ) {
	this->_change_op_nick(name);
	this->_name = name;

}

void User::set_realname( std::string name ) {
	this->_realname = name;
}

void User::set_fd(int fd)
{
	_socketfd = fd;
}

void User::set_ip( std::string ip ) {
	this->_ip_address = ip;
}

std::string User::get_ip( void ) {
	return (this->_ip_address);
}


const std::string User::get_name( void ) const {
	return (this->_name);
}

const std::string User::get_realname( void ) const {
	return (this->_realname);
}


int			User::get_socketfd( void ) const {
	return (this->_socketfd);
}

std::vector<Channel *>& User::get_list_channel() {
    return _joined_channel;
}

void User::add_channel_list(Channel& channel) {
	_joined_channel.push_back(&channel);
}

void User::remove_channel_list(Channel *channel) {
	const size_t len = _joined_channel.size();
		for (size_t i = 0; i < len; i++) {
			if (_joined_channel[i]->get_name() == channel->get_name()) {
				_joined_channel.erase(_joined_channel.begin() + i);
				return ;
			}
	}

	return ;
	std::vector<Channel*>::iterator it = find(this->_joined_channel.begin(), _joined_channel.end(), channel);
	_joined_channel.erase(it);
}

bool User::password_passed( void ) {
    return _password_passed;
}

void User::pass_password( void ) {
	_password_passed = true;
}

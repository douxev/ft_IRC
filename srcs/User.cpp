#include "User.hpp"

User::User( void ) {
	this->_name = "";
	this->_quit_message = "";
	this->_join_message = "";
	this->_socketfd = 0;
	this->_ip_address = 0;
}

User::User( const User& Other ) {
	this->_name = Other._name;
	this->_quit_message = Other._quit_message;
	this->_join_message = Other._join_message;
	this->_socketfd = Other._socketfd;
	this->_ip_address = Other._ip_address;
}

User& User::operator=( const User& Other ) {
	this->_name = Other._name;
	this->_quit_message = Other._quit_message;
	this->_join_message = Other._join_message;
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

void User::set_quit_message( std::string msg ) {
	this->_quit_message = msg;
}

void User::set_join_message( std::string msg ) {
	this->_join_message = msg;
}

void User::change_name( std::string name ) {
	this->_name = name;
}


std::string User::get_name( void ) const {
	return (this->_name);
}

std::string	User::get_quit_message( void ) const {
	return (this->_quit_message);
}
std::string User::get_join_message( void ) const {
	return (this->_join_message);
}

int			User::get_socketfd( void ) const {
	return (this->_socketfd);
}

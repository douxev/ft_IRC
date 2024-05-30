#include "User.hpp"

User::User( void ) {
	this->_name = "";
	this->_socketfd = 0;
	this->_ip_address = 0;
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

void User::change_name( std::string name ) {
	this->_name = name;
}


std::string User::get_name( void ) const {
	return (this->_name);
}

int			User::get_socketfd( void ) const {
	return (this->_socketfd);
}

#include "User.hpp"

User::User( void ) {

}

User::User( const User& Other ) {

}

User& User::operator=( const User& Other ) {

}

bool User::operator==( const User& Other ) {
	if (this->_name == Other._name)
		return (true);
	return (false);
}


User::~User() {

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
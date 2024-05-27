#include "Channel.hpp"

Channel::Channel( void ) {

}

Channel::Channel( const Channel& Other ) {

}

Channel& Channel::operator=( const Channel& Other ) {

}

Channel::~Channel() {

}

void Channel::send_message( const User& user, const std::string msg ) {

}

void Channel::change_role( const User& user, const User& target, bool is_op ) {

}

void Channel::user_join( const User& user ) {
	this->_add_connected_user(user);
	this->send_message(user, user.get_join_message());
}


void Channel::user_quit( const User& user ) {
	this->_remove_connected_user(user);
	this->send_message(user, user.get_quit_message());
}

void Channel::user_kicked( const User& user, std::string kick_message ) {
	this->_remove_connected_user(user);
	this->send_message(user, kick_message);
}

void Channel::_remove_connected_user( const User& user ) {
	const size_t len = this->_connected_users.size();
	for (size_t i = 0; i < len; i++) {
		if (this->_connected_users[i] == user) {
			this->_connected_users.erase(this->_connected_users.begin() + i);
			break ;
		}
	}
}

void Channel::_add_connected_user( const User& user ) {
	this->_connected_users.push_back(user);
}

//sset OP
void Channel::set_mode( t_enum_modes mode, User& user, std::string target, std::string value ) {
	switch (mode)
	{
	case OP:
		// check if user is op
		// change_role(user, value);
		break;
	
	default:
		break;
	}
}

//SET PASSWORD
void Channel::set_mode( t_enum_modes mode, bool value, std::string password ) {
	if ()
}

void Channel::set_mode( t_enum_modes mode, size_t value ) {
	switch (mode)
	{
	case INVITE:
		this->_modes.invite_only = value;
		break ;
	case TOPIC:
		this->_modes.op_topic = value;
		break ;
	case LIMIT:
		this->_modes.limit = value;
		break ;
	default:
		break;
	}
}


bool Channel::_is_op( const User& user ) {
	const size_t len = this->_op_users.size();
	for (size_t i = 0; i < len; i++) {
		if (this->_op_users[i] == user.get_name())
			return (true);
	}
	return (false);
}
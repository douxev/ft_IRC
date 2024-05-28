#include "Channel.hpp"
#include "utils.tpp"
#include <sys/socket.h>

Channel::Channel( void ) {

}

Channel::Channel( const Channel& Other ) {

}

Channel& Channel::operator=( const Channel& Other ) {

}

Channel::~Channel() {

}

void Channel::server_message( const std::string msg ) {
	const size_t len = this->_connected_users.size();
	for (size_t i = 0 ; i < len; i++) {
		send(this->_connected_users[i].get_socketfd(), \
		("SERVER: " + msg + "\n").c_str(), msg.size(), \
		0);
	}
}

void Channel::send_message( const User& user, const std::string msg ) {
	const size_t len = this->_connected_users.size();
	for (size_t i = 0 ; i < len; i++) {
		send(this->_connected_users[i].get_socketfd(), \
		(user.get_name() + ": " + msg + "\n").c_str(), \
		msg.size(), 0);
	}
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

void Channel::change_role( const User& user, const User& target, bool is_op ) {
	this->set_mode(OP, user, target.get_name(), is_op);
}

//Operators change modes
void Channel::set_mode( t_enum_modes mode, const User& user, const std::string target, bool value ) {
	if (mode != OP)
		return ;
	if (this->_is_op(user)) {
		if (value == false && this->_is_op(target)) {
			const size_t len = this->_op_users.size();
			for (size_t i = 0; i < len; i++) {
				if (this->_op_users[i] == target) {
					this->_op_users.erase(this->_op_users.begin() + i);
				break ;
				}
			}
		}
		else if (value == true) {
			this->_op_users.push_back(target);
		}
	}
}

//Password KEY
void Channel::set_mode( t_enum_modes mode, bool value, std::string password ) {
	if (mode != KEY)
		return ;
	if (!value) {
		this->_modes.password.clear();
	}
	else {
		this->_modes.password = password;
	}
}

//Invite, Topics, Limits
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

bool Channel::_is_op( const std::string user ) {
	const size_t len = this->_op_users.size();
	for (size_t i = 0; i < len; i++) {
		if (this->_op_users[i] == user)
			return (true);
	}
	return (false);
}

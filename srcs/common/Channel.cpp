#include "ft_irc.hpp"
#include "utils.tpp"
#include <cstddef>
#include <sys/socket.h>

Channel::Channel( void ) {
	this->_modes.invite_only = 0;
	this->_modes.op_topic = 0;
	this->_name = "Unknown";
	this->_modes.password = "";
	this->_modes.limit = 0;
}

Channel::Channel( const std::string name, const User& user, std::string join_message ) {
	this->user_join(user, join_message);
	this->_op_users.push_back(user.get_name());
	this->_name = name;
	this->_modes.invite_only = 0;
	this->_modes.op_topic = 0;
	this->_modes.password = "";
	this->_modes.limit = 0;
}

Channel::Channel( const Channel& Other ):
_modes(Other._modes), _topic(Other._topic), _name(Other._name), _op_users(Other._op_users), _connected_users(Other._connected_users) {}

Channel& Channel::operator=( const Channel& Other ) {
	this->_modes = Other._modes;
	this->_topic = Other._topic;
	this->_op_users = Other._op_users;
	this->_connected_users = Other._connected_users;
	this->_name = Other._name;
	return (*this);
}

bool Channel::operator==( const Channel& Other ){
	if (this->_name == Other._name)
		return (true);
	return (false);

}

Channel::~Channel() {

}

void	Channel::send_userlist( const User& user ) {
	std::stringstream msg_to_send;

	msg_to_send << "353 " + this->get_name() + " ";
	const size_t len = this->_connected_users.size();
	for (size_t i = 0; i < len; i++) {
		if (this->_is_op(this->_connected_users[i]))
			msg_to_send << "@";
		msg_to_send << this->_connected_users[i].get_name() << " ";
	}
	ft_send(user.get_socketfd(), msg_to_send.str());
	ft_send(user.get_socketfd(), "366 " + this->get_name() + " :End of /NAMES list");
}

void Channel::user_join( const User& user ) {
	this->_add_connected_user(user);

	std::stringstream msg_to_send;
	msg_to_send << ":" << user.get_name() << " JOIN #" << this->_name;
	std::cout << msg_to_send.str();
	this->send_channel(msg_to_send.str());
	ft_send(user.get_socketfd(), "TOPIC #" + this->get_name() + " :" + this->_topic);
	this->send_userlist(user);
}

void Channel::user_quit( const User& user, const std::string quit_message ) {
	this->_remove_connected_user(user);
	this->send_channel(":" + user.get_name() + " PART #" + this->get_name() + 
						" :" + quit_message);
}

void Channel::user_kicked( const User& user, const User& target, std::string kick_message ) {
	this->_remove_connected_user(user);
	this->send_channel(":" + user.get_name() + " KICK #" + this->get_name() + 
						" " + target.get_name() + " :" + kick_message);
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

void	Channel::force_op( const User& user) {
	this->_op_users.push_back(user.get_name());
}

bool Channel::_is_op( const std::string user ) {
	const size_t len = this->_op_users.size();
	for (size_t i = 0; i < len; i++) {
		if (this->_op_users[i] == user)
			return (true);
	}
	return (false);
}

std::string Channel::get_name( void ) {
	return (this->_name);
}

void Channel::send_channel( const std::string msg ) {
	const size_t len = this->_connected_users.size();

	for (size_t i = 0; i < len; i++) {
		ft_send(this->_connected_users[i].get_socketfd(), msg);
	}
}

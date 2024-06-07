#include "ft_irc.hpp"
#include "utils.tpp"
#include <cstddef>
#include <sstream>
#include <sys/socket.h>
#include "Channel.hpp"
#include "numeric_replies.hpp"

Channel::Channel( void ) {
	this->_modes.invite_only = 0;
	this->_modes.op_topic = 0;
	this->_name = "Unknown";
	this->_modes.password = "";
	this->_modes.limit = 0;
	this->_topic = "";
	this->_topic_whotime = "";
}

Channel::Channel( const std::string name, User& user ) {
	
	this->_name = name;
	this->_modes.invite_only = 0;
	this->_modes.op_topic = 0;
	this->_modes.password = "";
	this->_modes.limit = 0;
	this->_topic = "";
	this->_topic_whotime = "";
	this->user_join(user, "");
	this->force_op(user);
}

Channel::Channel( const Channel& Other ):
_modes(Other._modes), _topic(Other._topic), _name(Other._name), _op_users(Other._op_users), _invited(Other._invited), _connected_users(Other._connected_users) {}

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
	const size_t len = this->_connected_users.size();
	std::stringstream msg_to_send;

	msg_to_send << "353 " + user.get_name() << " " << this->get_name() << " :";

	for (size_t i = 0; i < len; i++) {
		if (this->is_op(*this->_connected_users[i]))
			msg_to_send << "@";
		msg_to_send << this->_connected_users[i]->get_name() << " ";
	}
	ft_send(user.get_socketfd(), msg_to_send.str() + "\n");
	ft_send(user.get_socketfd(), "366 " + user.get_name() + " " + this->get_name() + " :End of /NAMES list\n");
}

void Channel::send_who( Server& server, int reply_socket ) {
	const size_t len = this->_connected_users.size();

	User& user = server.get_user_class(reply_socket);
	for (size_t i = 0; i < len; i++) {
		// if (this->_connected_users[i].get_socketfd() != reply_socket)
		ft_send(reply_socket, 
			"352 " + user.get_name() + 
			" " + 
			this->_name + " " + 
			this->_connected_users[i]->get_name() + " " + 
			this->_connected_users[i]->get_ip() + " " + 
			server.get_ip() + " " + 
			this->_connected_users[i]->get_name() + " H : 0 " + 
			this->_connected_users[i]->get_realname() + "\n");
	}
	ft_send(reply_socket, "315 " + user.get_name() + " :End of WHO list");
}

void Channel::user_join( User& user, std::string pass ) {

	if (this->_modes.invite_only && !this->is_invited(user)) {
		ft_send(user.get_socketfd(), ERR_INVITEONLYCHAN + user.get_name() + " " + 
			this->get_name() + " :Invite Only (+i)\n");
		return ; //!NO INVITE
	}
	if (this->_modes.limit && this->_connected_users.size() == this->_modes.limit) {
		ft_send(user.get_socketfd(), ERR_CHANNELISFULL + user.get_name() + " " + 
			this->get_name() + " :Channel is Full (+l)\n");
		return ; //!USER LIMIT REACHED
	}
	if (this->_modes.password.empty() || pass == this->_modes.password) {
		this->_add_connected_user(user);
		user.add_channel_list(this);
		this->send_userlist(user);
		if (this->is_invited(user.get_name()))
			this->remove_invited(user.get_name());
		return ; //!EVERYTHING GOOD
	}
	else {
		ft_send(user.get_socketfd(), ERR_BADCHANNELKEY + user.get_name() + " " + 
			this->get_name() + " :Bad Channel Key (+k)\n");
		return ; //!Password Mismatch
	}
}

void Channel::user_quit( User& user, const std::string quit_message ) {
	this->send_channel(user.get_socketfd(), ":" + user.get_name() + " QUIT :Quit: " + quit_message);
	this->_remove_connected_user(user);
}

void Channel::change_op_nick( const std::string user, const std::string new_name ) {
	const size_t len = this->_op_users.size();

	for (size_t i = 0; i < len; i++) {
		if (this->_op_users[i] == user) {
			this->_op_users[i] = new_name;
			return ;
		}
	}
}

void Channel::user_part( User& user, const std::string part_message ) {
	this->_remove_connected_user(user);
	this->send_channel(user.get_socketfd(), ":" + user.get_name() + " PART " + this->get_name() + 
						" :" + part_message);
}

void Channel::user_kicked( User& user, const User& target, std::string kick_message ) {
	this->_remove_connected_user(user);
	this->send_channel(user.get_socketfd(), ":" + user.get_name() + " KICK #" + this->get_name() + 
						" " + target.get_name() + " :" + kick_message);
}

std::string Channel::user_count( void ) {
	std::stringstream input;
	input << this->_connected_users.size();
	return (input.str());
}

void Channel::_remove_connected_user( User& user ) {
	const size_t len = this->_connected_users.size();
	if (is_op(user.get_name()))
		this->_op_users.erase(std::remove(this->_op_users.begin(), this->_op_users.end(), user.get_name()));

	this->_connected_users.erase(std::remove(this->_connected_users.begin(), this->_connected_users.end(), &user), this->_connected_users.end());
}

void Channel::_add_connected_user( User& user ) {
	this->_connected_users.push_back(&user);
}

void Channel::change_role( const User& user, const User& target, bool is_op ) {
	this->set_mode(OP, user, target.get_name(), is_op);
}

//Operators change modes
void Channel::set_mode( t_enum_modes mode, const User& user, const std::string target, bool value ) {
	if (mode != OP)
		return ;
	if (this->is_op(user)) {
		if (value == false && this->is_op(target)) {
			std::cout << "IS DEOPING SOMEONE" << std::endl;
			const size_t len = this->_op_users.size();
			for (size_t i = 0; i < len; i++) {
				if (this->_op_users[i] == target) {
					this->_op_users.erase(this->_op_users.begin() + i);
				break ;
				}
			}
		}
		else if (value == true) {
			std::cout << "IS OPING SOMEONE" << std::endl;
			this->_op_users.push_back(target);
		}
	}
}

//Password KEY
void Channel::set_mode( t_enum_modes mode, bool value, std::string password ) {
	if (mode != KEY)
		return ;
	if (!value) {
		std::cout << "REMOVING PASSWORD CHANNEL" << std::endl;
		this->_modes.password.clear();
	}
	else {
		std::cout << "ADDING PASSWORD CHANNEL" << std::endl;
		this->_modes.password = password;
	}
}

//Invite, Topics, Limits
void Channel::set_mode( t_enum_modes mode, size_t value ) {
	switch (mode)
	{
	case INVITE:
		std::cout << "CHANGING INVITE ONLY" << std::endl;
		this->_modes.invite_only = value;
		break ;
	case TOPIC:
		std::cout << "CHANGING OPTOPIC" << std::endl;
		this->_modes.op_topic = value;
		break ;
	case LIMIT:
		std::cout << "CHANGING LIMIT ONLY" << std::endl;
		this->_modes.limit = value;
		break ;
	default:
		break;
	}
}

void	Channel::print_ops( void ) {
	const size_t len = this->_op_users.size();
	
	std::cout << "OP USERS: ";
	for (size_t i = 0; i < len; i++) {
	std::cout << this->_op_users[i] << " ";
	}
	std::cout << std::endl;
}

bool Channel::topic_mode_is_off () {

	if (this->_modes.op_topic == false)
		return (true);
	return (false);
}

bool Channel::is_op( const User& user ) {
	const size_t len = this->_op_users.size();
	for (size_t i = 0; i < len; i++) {
		if (this->_op_users[i] == user.get_name())
			return (true);
	}
	return (false);
}

void	Channel::force_op( const User& user) {
	if (!this->is_op(user))
		this->_op_users.push_back(user.get_name());
}

bool Channel::is_op( const std::string user ) {
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

std::string Channel::get_topic( void ) {
	return (this->_topic);
}

void Channel::send_channel( const std::string msg ) {
	const size_t len = this->_connected_users.size();
	for (size_t i = 0; i < len; i++) {
		ft_send(this->_connected_users[i]->get_socketfd(), msg);
	}
}

void Channel::send_channel( int sender_fd, const std::string msg ) {
	const size_t len = this->_connected_users.size();
	for (size_t i = 0; i < len; i++) {
		if (this->_connected_users[i]->get_socketfd() != sender_fd)
			ft_send(this->_connected_users[i]->get_socketfd(), msg);
	}
}

bool Channel::is_on_channel( const std::string username ) {
	const size_t len = this->_connected_users.size();

	for (size_t i = 0; i < len; i++) {
		if (this->_connected_users[i]->get_name() == username)
			return (true);
	}
	return (false);
}

void Channel::set_topic( std::string topic, std::string topic_whotime ) {
	this->_topic = topic;
	this->_topic_whotime = topic_whotime;
}

void	Channel::add_invited( const std::string user ) {
	if (!this->is_invited(user))
		this->_invited.push_back(user);
}

void	Channel::remove_invited( const std::string user ) {
	const size_t len = this->_invited.size();
	for (size_t i = 0; i < len; i++) {
		if (this->_invited[i] == user) {
			this->_invited.erase(this->_invited.begin() + i);
			return ;
		}
	}
}


bool	Channel::is_invited( const User& user ) {
	const size_t len = this->_invited.size();

	for (size_t i = 0; i < len; i++) {
		if (this->_invited[i] == user.get_name())
			return (true);
	}
	return (false);
}

bool	Channel::is_invited( const std::string user ) {
	const size_t len = this->_invited.size();

	for (size_t i = 0; i < len; i++) {
		if (this->_invited[i] == user)
			return (true);
	}
	return (false);
}

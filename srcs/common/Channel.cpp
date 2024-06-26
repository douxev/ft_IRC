#include "ft_irc.hpp"
#include "utils.tpp"
#include <cstddef>
#include <sstream>
#include <sys/socket.h>
#include "Channel.hpp"
#include "numeric_replies.hpp"
#include <algorithm>

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
	this->force_op(user);
	this->user_join(user, "");
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
	this->_invited.clear();
	this->_op_users.clear();
	this->_connected_users.clear();
}

void	Channel::send_userlist( const User& user ) {
	const size_t len = this->_connected_users.size();
	std::stringstream msg_to_send;

	msg_to_send << "353 " + user.get_name()  + "!" + user.get_realname() + "@" + user.get_ip() << " = " << this->get_name() << " :";

	for (size_t i = 0; i < len; i++) {
		if (this->is_op(this->_connected_users[i]->get_name()))
			msg_to_send << "@";
		msg_to_send << this->_connected_users[i]->get_name() << " ";
	}
	ft_send(user.get_socketfd(), msg_to_send.str() + "\r\n");
	ft_send(user.get_socketfd(), "366 " + user.get_name() + "!" + user.get_realname() + "@" + user.get_ip() + " " + this->get_name() + " :End of /NAMES list\r\n");
}

void Channel::send_who( Server& server, int reply_socket ) {
	const size_t len = this->_connected_users.size();

	User& user = server.get_user_class(reply_socket);
	for (size_t i = 0; i < len; i++) {
		// if (this->_connected_users[i].get_socketfd() != reply_socket)
		std::string msg = "";
		msg =	"352 " + user.get_name() + "!" + user.get_realname() + "@" + user.get_ip() +
				" " + 
				this->_name + " " + 
				this->_connected_users[i]->get_name() + " " + 
				this->_connected_users[i]->get_ip() + " " + 
				server.get_ip() + " " + 
				this->_connected_users[i]->get_name() + " H";

		if (this->is_op(*this->_connected_users[i]))
			msg += "@";
		ft_send(reply_socket, 
			msg + " :0 " + 
			this->_connected_users[i]->get_realname() + "\r\n");
	}
	ft_send(reply_socket, "315 " + user.get_name() + "!" + user.get_realname() + "@" + user.get_ip() + " :End of WHO list\r\n");
}

void Channel::user_join( User& user, std::string pass ) {

	if (this->_modes.invite_only && !this->is_invited(user)) {
		ft_send(user.get_socketfd(), ERR_INVITEONLYCHAN + user.get_name() + " " + 
			this->get_name() + " :Invite Only (+i)\r\n");
		return ; //!NO INVITE
	}
	if (this->_modes.limit && this->_connected_users.size() == this->_modes.limit) {
		ft_send(user.get_socketfd(), ERR_CHANNELISFULL + user.get_name() + " " + 
			this->get_name() + " :Channel is Full (+l)\r\n");
		return ; //!USER LIMIT REACHED
	}
	if (this->_modes.password.empty() || pass == this->_modes.password) {
		this->_add_connected_user(user);
		user.add_channel_list(*this);
		this->send_userlist(user);
		if (this->is_invited(user.get_name()))
			this->remove_invited(user.get_name());
		return ; //!EVERYTHING GOOD
	}
	else {
		ft_send(user.get_socketfd(), ERR_BADCHANNELKEY + user.get_name() + " " + 
			this->get_name() + " :Bad Channel Key (+k)\r\n");
		return ; //!Password Mismatch
	}
}

void Channel::user_quit( User& user, const std::string quit_message ) {
	this->send_channel(user.get_socketfd(), ":" + user.get_name() + " QUIT :" + quit_message);
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
	this->send_channel(":" + user.get_name() + "!" + user.get_realname() + "@" + user.get_ip() + " PART " + this->get_name() + " :" + part_message + "\r\n");
	// ft_send(user.get_socketfd(), "404 " + user.get_name() + " " + this->get_name() + " :Parted from Channel\r\n");
	this->_remove_connected_user(user);
}

void Channel::user_kicked( User& user, User& target, std::string kick_message ) {
	(void) target;
	(void) kick_message;
	(void) user;
	this->_remove_connected_user(target);
	user.remove_channel_list(this);
}

std::string Channel::user_count( void ) {
	std::stringstream input;
	input << this->_connected_users.size();
	return (input.str());
}

void Channel::_remove_connected_user( User& user ) {
	if (is_op(user.get_name())) {
		this->_op_users.erase(std::remove(this->_op_users.begin(), this->_op_users.end(), user.get_name()));
	}
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
	if (mode != OP || (value == false && !this->is_op(target)) || (value == true && this->is_op(target)))
		return ;
	if (this->is_op(user)) {
		if (value == false && this->is_op(target)) {
			std::cout << SERVER_INFO << target << " is no longer OP on " << this->_name << std::endl;
			const size_t len = this->_op_users.size();
			for (size_t i = 0; i < len; i++) {
				if (this->_op_users[i] == target) {
					this->_op_users.erase(this->_op_users.begin() + i);
				break ;
				}
			}
		}
		else if (value == true && !this->is_op(target)) {
			std::cout << SERVER_INFO << target << " is now OP on " << this->_name << std::endl;
			this->_op_users.push_back(target);
		}
	}
}

//Password KEY
void Channel::set_mode( t_enum_modes mode, bool value, std::string password ) {
	if (mode != KEY || (!value && this->_modes.password.empty()))
		return ;
	if (!value) {
		std::cout << SERVER_INFO << "channel password cleared on " << this->_name << std::endl;
		this->_modes.password.clear();
	}
	else {
		std::cout << SERVER_INFO << "channel password added on " << this->_name << std::endl;
		this->_modes.password = password;
	}
}

//Invite, Topics, Limits
void Channel::set_mode( t_enum_modes mode, size_t value ) {
	switch (mode)
	{
	case INVITE:
		if (this->_modes.invite_only == value)
			break;
		std::cout << SERVER_INFO << "invite only " << (value ? "ON": "OFF") << " on " << this->_name << std::endl;
		this->_modes.invite_only = value;
		break ;
	case TOPIC:
		if (this->_modes.op_topic == value)
			break;
		std::cout << SERVER_INFO << "topic can be change by mod only " << (value ? "ON": "OFF") << " on " << this->_name << std::endl;
		this->_modes.op_topic = value;
		break ;
	case LIMIT:
		if ((this->_modes.limit == false && value == false))
			break;
		std::cout << SERVER_INFO << "changing limits on " << this->_name << " to " << value << std::endl;
		this->_modes.limit = value;
		break ;
	default:
		break;
	}
}

std::string	Channel::get_modes( void ) {

	std::stringstream modes;
	if (this->_modes.invite_only == 1 || this->_modes.op_topic == 1 || this->_modes.limit != 0 || 
		this->_modes.password != "" || this->_modes.limit != 0)
		modes << "+";

	if (this->_modes.invite_only == 1)
		modes << "i";
	if (this->_modes.op_topic == 1)
		modes <<"t";
	if (this->_modes.limit != 0)
		modes <<"l";
	if (this->_modes.password != "")
		modes <<"k";
	if (this->_modes.limit != 0) {
		modes <<" " << this->_modes.limit;
	}

	return ( modes.str() + "\n" );
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

//adds ':user ' before sending it
void Channel::send_channel_client( const std::string msg ) {
	const size_t len = this->_connected_users.size();
	for (size_t i = 0; i < len; i++) {
		ft_send(this->_connected_users[i]->get_socketfd(), ":" + this->_connected_users[i]->get_name() + " " + msg);
	}
}

void Channel::refresh_topic( void ) {
	const size_t len = this->_connected_users.size();
	for (size_t i = 0; i < len; i++) {
		ft_send(this->_connected_users[i]->get_socketfd(), 
			RPL_TOPIC + this->_connected_users[i]->get_name() + " " + 
			this->_name + " :" + this->_topic + "\r\n");
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

void Channel::set_topic( std::string topic ) {
	this->_topic = topic;
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
	if (std::find(this->_invited.begin(), this->_invited.end(), user) == this->_invited.end())
		return false;
	return true;
}

size_t	Channel::get_size( void ) {
	return (this->_connected_users.size());
}

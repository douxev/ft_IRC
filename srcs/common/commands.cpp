#include "Channel.hpp"
#include "error_numeric.hpp"
#include "exceptions.hpp"
#include "ft_irc.hpp"
#include "numeric_replies.hpp"
#include <cstddef>
#include <sstream>
#include <string>
#include "Server.hpp"
#include <ctime>
#include <vector>
#include <algorithm>

void	pong(int reply_socket, std::string message) {
	ft_send(reply_socket, "PONG " + message + "\r\n");
}

void	motd_command( Server& server, int reply_socket ) {
	if (server.get_motd().empty())
		ft_send(reply_socket, "422 " + server.get_user_class(reply_socket).get_name() + " :No MOTD set\r\n");
	else
		ft_send(reply_socket, "375 " + server.get_user_class(reply_socket).get_name() + " :Message of the Day \r\n372 :" + 
				server.get_motd() + "\r\n376 " + server.get_user_class(reply_socket).get_name() + " :End of MOTD.\r\n");
}

void	version_command( int reply_socket ) {
	ft_send(reply_socket, RPL_VERSION);
	ft_send(reply_socket, RPL_ISUPPORT);
}

void	nick_command( Server& server, int reply_socket, std::string message ) {
	//checks needed here change if not already taken by SOMEONE ELSE
	User& user = server.get_user_class(reply_socket);
	std::string oldnick = user.get_name();
	
	try {
		server.change_nick(user, message);
	}
	catch (const NickAlreadyTakenException& e) {
		ft_send(reply_socket, ERR_NICKNAMEINUSE + oldnick + " " + message + " :Nickname is already in use\r\n");
		return ;
	}
	server.send_all(":" + oldnick + " NICK " + user.get_name() + "\r\n");
	const size_t len = user.get_list_channel().size();
	for (size_t i = 0; i < len; i++) {
		user.get_list_channel()[i]->send_who(server, reply_socket);
	}
}

void	cap_command( Server& server, int reply_socket, std::istringstream &message ) {
	(void) server;

	return ;
	std::string	param;
	std::getline(message, param, ' ');
	if (param == "LS")
		ft_send(reply_socket, "CAP * LS :\r\n");
	else if (param == "LIST")
		ft_send(reply_socket, "CAP * LIST :\r\n");
}

void	join_command( Server& server, int reply_socket, std::istringstream &message ) {
	std::string password;
	std::string channel;

	std::getline(message, channel, ' ');
	std::getline(message, password, ' ');
	

	try {
		if (server.get_channel_class(channel).is_on_channel(server.get_user_class(reply_socket).get_name()))
			return ;
	}
	catch (const NoSuchChannelException& e) {
	}


	server.join_channel(server.get_user_class(reply_socket).get_name(), 
						channel, password);
	
	if (!server.is_on_channel(channel, server.get_user_class(reply_socket).get_name()))
		return ;

	// names_command(server, reply_socket, channel);

	if (server.get_channel_class(channel).get_topic().empty())
		no_topic_set(reply_socket, channel);
	else {
		ft_send(reply_socket, RPL_TOPIC + server.get_user_class(reply_socket).get_name() + 
			" " + channel + " :" + server.get_channel_class(channel).get_topic() + "\r\n");
		std::cout << RED << "Topic is: " << RESET << server.get_channel_class(channel).get_topic() << std::endl;
	}
}

void	privmsg_command( Server& server, int reply_socket, std::istringstream &message ) {

	if (find(message.str().begin(), message.str().end(), ':') == message.str().end()) {	//detection de l'absence de cible
		ft_send(reply_socket, "411 " + server.get_user_class(reply_socket).get_name() + " :No recipient given (PRIVMSG " + message.str() + " )\r\n");
		return ;
	}

	std::string recipients_list;
	std::getline(message, recipients_list, ':');
	std::string msg;
	std::getline(message, msg);
	if (!msg.size()) {
		ft_send(reply_socket, "412 " + server.get_user_class(reply_socket).get_name() + " :No text to send\r\n");
		return ;
	}

	std::istringstream stream;
	stream.str(recipients_list);
	for(std::string recipient; std::getline(stream, recipient, ',');) {
		if (recipient[recipient.size() - 1] == ' ')
			recipient.erase(recipient.end() - 1); //-1 pour enlever l'espace
		try
		{
			User user = server.get_user_class(recipient);
			ft_send(user.get_socketfd(), ":" + server.get_user_class(reply_socket).get_name() + " PRIVMSG " + recipient + " :" + msg + "\r\n");
		}
		catch(const std::exception& e)	//ce n'est pas un user
		{
			try
			{
				Channel channel = server.get_channel_class(recipient);
				if (channel.is_on_channel(server.get_user_class(reply_socket).get_name()))
					channel.send_channel(reply_socket, ":" + server.get_user_class(reply_socket).get_name() + " PRIVMSG " + recipient + " :" + msg + "\r\n");
				else
					ft_send(reply_socket, "404 " + server.get_user_class(reply_socket).get_name() + " " + channel.get_name() + " :Cannot send to channel\r\n");

			}
			catch(const std::exception& e) { //ce n'est pas un channel non plus
				ft_send(reply_socket, "401 " + server.get_user_class(reply_socket).get_name() + " " + recipient + " :No such nick/channel\r\n");
			}
		}
	}
	
}

/*    Command: MODE
		Parameters: <target> [<modestring> [<mode arguments>...]]*/
void	mode_command( Server& server, int reply_socket, std::istringstream &message ) {
	std::string		target;
	std::string		value;
	bool			op_sign = false;
	char			mode;

	std::getline(message, target, ' ');
	std::getline(message, value, ' ');


	if (value.size() < 2){
		if (target.size() > 1 && target.at(0) == '#') {
			ft_send(reply_socket, RPL_CHANNELMODEIS + 
				server.get_user_class(reply_socket).get_name() + " " + target + " " + server.get_channel_class(target).get_modes() + " \r\n");
		}
		else {
			ft_send(reply_socket, RPL_UMODEIS + 
				server.get_user_class(reply_socket).get_name() + " \r\n");
		}
		return ; //MODE QUERY
	}	


	if (value.at(0) == '+')
		op_sign = true;

	if (value.size() == 2 && value.at(1) == 'b') {
		ft_send(reply_socket, "368 " + server.get_user_class(reply_socket).get_name() + " " + target + " :End of channel banlist.\r\n");
		return ;
	}

	std::string password;
	std::getline(message, password, ' ');
	int size = std::strtol(password.c_str(), NULL, 0);

	for (size_t i = 1; i < value.size(); i++) {
		mode = value.at(i);
		try{
			if (!server.is_op(target, server.get_user_class(reply_socket).get_name())) {
				ft_send(reply_socket, (std::string) ERR_CHANOPRIVSNEEDED + target + " :You're not channel operator\r\n");
				return ;
			}
			(void) server.get_user_class(target);

		}
		catch(const NoSuchChannelException& e) {

			switch (mode)
			{
			case 'i':
				std::cout << SERVER_INFO << "Client '" << reply_socket << "' wants invisible role" << std::endl;
				break ;
			case 'o':
				server.get_channel_class(target).set_mode( OP,  server.get_user_class(reply_socket), password, op_sign );
				server.get_channel_class(target).send_channel(":" + server.get_user_class(reply_socket).get_name() 
					+ " MODE " + target + " " + value.at(0) + "o " + password + " \r\n");
				break ;
			default:
				ft_send(reply_socket, RPL_UMODEIS + server.get_user_class(reply_socket).get_name() + "\r\n");
			}

		}
		catch(const NoSuchNickException& e) {

			switch(mode)
			{
			case 'i':
				server.get_channel_class(target).set_mode( INVITE, op_sign );
				server.get_channel_class(target).send_channel(":" + server.get_user_class(reply_socket).get_name() 
					+ "!~" + server.get_user_class(reply_socket).get_realname() + "@" + server.get_user_class(reply_socket).get_ip() +
					"" + " MODE " + target + " " + value.at(0) + "i " + " \r\n");
				break ;
			case 't':
				server.get_channel_class(target).set_mode( TOPIC, op_sign );
				server.get_channel_class(target).send_channel(":" + server.get_user_class(reply_socket).get_name() 
					+ "!~" + server.get_user_class(reply_socket).get_realname() + "@" + server.get_user_class(reply_socket).get_ip() +
					"" + " MODE " + target + " " + value.at(0) + "t " + " \r\n");
				break ;
			case 'l':
				for (size_t y = 0; y < password.size(); y++) {
					if (isdigit(password[y]) == 0) {
						size = -1;
					}	
				}
				if (size == -1) {
					std::cout << "incorrect limit value" << std::endl;
					break;
				}
				else if (op_sign) {
					server.get_channel_class(target).set_mode( LIMIT, size );
					std::ostringstream oss;
					oss << size;
					std::string size_string = oss.str();
					server.get_channel_class(target).send_channel(":" + server.get_user_class(reply_socket).get_name() 
					+ "!~" + server.get_user_class(reply_socket).get_realname() + "@" + server.get_user_class(reply_socket).get_ip() +
					"" + " MODE " + target + " " + value.at(0) + "l " + size_string +  " \r\n");
				}
				else {
					server.get_channel_class(target).set_mode(LIMIT, op_sign);
					server.get_channel_class(target).send_channel(":" + server.get_user_class(reply_socket).get_name() 
					+ "!~" + server.get_user_class(reply_socket).get_realname() + "@" + server.get_user_class(reply_socket).get_ip() +
					"" + " MODE " + target + " " + value.at(0) + "l " + "0" + " \r\n");
				}
				break ;
			case 'k':
				server.get_channel_class(target).set_mode(KEY, op_sign, password);
				server.get_channel_class(target).send_channel(":" + server.get_user_class(reply_socket).get_name() 
					+ "!~" + server.get_user_class(reply_socket).get_realname() + "@" + server.get_user_class(reply_socket).get_ip() +
					"" + " MODE " + target + " " + value.at(0) + "k " + " \r\n");
				break ;
			case 'o':
				server.get_channel_class(target).set_mode( OP,  server.get_user_class(reply_socket), password, op_sign );
				server.get_channel_class(target).send_channel(":" + server.get_user_class(reply_socket).get_name() 
					+ " MODE " + target + " " + value.at(0) + "o " + password + " \r\n");
				break ;
			default:
				std::cout << "Mode not recognized, is: [" << mode << "]" << std::endl;
				break;
			}
		}
	if (value.at(i) == 'o' || value.at(i) == 'k' || value.at(i) == 'b' || value.at(i) == 'l')
		break;
	}
}

//send_who from channel
void	who_command( Server& server, int reply_socket, std::istringstream &message ) {
	server.get_channel_class(message.str()).send_who(server, reply_socket);
}

void	topic_command( Server& server, int reply_socket, std::istringstream &message ) {
	std::string	channel;
	std::stringstream msg_to_send;
	std::getline(message, channel, ' ');
	std::string topic_message;
	std::getline(message, topic_message, ':');
	std::getline(message, topic_message);
	const std::string user = server.get_user_class(reply_socket).get_name();

if (channel.empty())
	ft_send(reply_socket, ERR_NEEDMOREPARAMS + user + "TOPIC :Not enough parameters\r\n");

try {
	if (server.is_on_channel(channel, user)) {
		if (server.is_op(channel, user) || server.get_channel_class(channel).topic_mode_is_off() == true || topic_message.empty()) {
			if (topic_message.empty()) {
				if (server.get_topic(channel).empty()) {
					no_topic_set(reply_socket, message.str());
					return ;
				}
				else {
					msg_to_send << RPL_TOPIC << server.get_user_class(reply_socket).get_name() << channel << "\r\n";
					if (ft_send(reply_socket, msg_to_send.str()) == -1)
						std::cerr << SERVER_INFO << "Send error to client " << server.get_user_class(reply_socket).get_name() << ": " <<  strerror(errno)  << std::endl;
					return ;
				}
			}
			else {
				std::cout << SERVER_INFO << channel << " topic is now: " << topic_message << std::endl;
				server.get_channel_class(channel).set_topic(topic_message);
				server.get_channel_class(channel).refresh_topic();
				return ;
			}
		}
		else {
			if (!server.is_op(channel, user)) {
				throw ChanOPrivsNeededException();
			}
		}
	}
	else
		ft_send(reply_socket, "442 " + user + " " + channel + "You are not on channel " + channel + "\r\n");
	}
catch(const NoSuchChannelException& e) {
	ft_send(reply_socket, ERR_NOSUCHCHANNEL + user + " " + channel + " :No such channel " + channel + "\r\n");
}
}

//NAMES => list all channel and their occupant, then all users outside any channel, under the "channel *"
//NAMES #CHAN1,#CHAN2 => list all users on channel(s)
void	names_command( Server& server, int reply_socket, std::string message ) { 
	std::istringstream iss(message);
	names_command(server, reply_socket, iss);
}

void	names_command( Server& server, int reply_socket, std::istringstream &message ) {
	//names
	(void) server;
	(void) reply_socket;
	(void) message;

	if (!server.get_connected_user().size() || !server.get_channels_list().size())
		return ;

	int i = 0;	// le nombre de channel passés en parametre
	User user = server.get_user_class(reply_socket);

	for (std::string channel_name; std::getline(message, channel_name, ',');i++) {
		try {
			server.get_channel_class(channel_name).send_userlist(user);
		}
		catch(const std::exception& e) {
			ft_send(reply_socket, "366 " + user.get_name() + " " + channel_name + " :End of /NAMES list\r\n");
		}
	}
	if (!i ) //aucuns parametres
	{
		std::vector<Channel*> channel_list = server.get_channels_list();
		for (int j = 0; channel_list[j] != channel_list.back(); j++) {
			channel_list[j]->send_userlist(user);
		}
		std::vector<User*> user_list = server.get_connected_user();
		for (int j = 0; user_list[j] < user_list.back(); j++) {
			if (!user_list[j]->get_list_channel().size())
				ft_send(reply_socket, "353 " + user.get_name() + " * :" + user_list[j]->get_name() + "\r\n");
		}
		ft_send(reply_socket, "366 " + user.get_name() + " * :End of /NAMES list\r\n");
	}
}

void	list_command( Server& server, int reply_socket, std::istringstream &message ) {
	std::string channel;
	std::getline(message, channel, ' ');

	const User& user = server.get_user_class(reply_socket);
	const std::vector<Channel *> chans = server.get_channels_list();
	const size_t len = chans.size();
	for (size_t i = 0; i < len; i++) {
		std::cout << channel.size() << " " << channel << std::endl;
		if (channel.empty() || chans[i]->get_name() == channel)
			ft_send(reply_socket, RPL_LIST + user.get_name() + " " + chans[i]->get_name() + " " +
		chans[i]->user_count() + " :" + "TOPIC: " + chans[i]->get_topic() + "\r\n");
	}
	ft_send(reply_socket, RPL_LISTEND + user.get_name() + " :End of /LIST\r\n");
}

void	whois_command( Server& server, int reply_socket, std::string message ) {
	std::istringstream msg(message);
	whois_command(server, reply_socket, msg);
}


void	whois_command( Server& server, int reply_socket, std::istringstream &message ) {
	const User& user = server.get_user_class(reply_socket);
	std::string target;

	if (message.str().empty()) {
		ft_send(reply_socket, ERR_NONICKNAMEGIVEN + user.get_name() + " :No nickname given\r\n");
		return ;
	}
	
	if (!std::getline(message, target))
		return ;
	try {
		ft_send(reply_socket, RPL_WHOISUSER + user.get_name() + " " + target + " " + target + " " +
			server.get_user_class(target).get_ip() + " * :" + server.get_user_class(target).get_realname() + "\r\n"); // whoisUSER
		const size_t len = server.get_user_class(target).get_list_channel().size();
		User& target_u = server.get_user_class(target);
		const std::vector<Channel *> chans = target_u.get_list_channel();
		std::string whois_channel_string = RPL_WHOISCHANNELS + user.get_name() + " " + target + " :" ;
		for (size_t i = 0; i < len; i++) {
			if (chans[i]->is_op(target_u))
				whois_channel_string += "@";
			whois_channel_string += chans[i]->get_name() + " ";
		}
		ft_send(reply_socket, whois_channel_string + "\r\n");
		ft_send(reply_socket, RPL_ENDOFWHOIS + user.get_name() + " " + target + " :End of /WHOIS list\r\n");
	}
	catch(const NoSuchNickException& e) {
		ft_send(reply_socket, ERR_NOSUCHNICK + user.get_name() + " " + target + " :No such nick\r\n");
		return ;
	}
}

void	invite_command( Server& server, int reply_socket, std::istringstream &message ) {
	(void) server;
	(void) reply_socket;
	(void) message;

	std::string username;
	std::string channel;

	if (message.str().empty()) {
		ft_send(reply_socket, ERR_NEEDMOREPARAMS + server.get_user_class(reply_socket).get_name() + " INVITE :Not enough parameters\r\n");
		return ;
	}
	std::getline(message, username, ' ');
	std::getline(message, channel);
	if (!server.is_on_channel(channel, server.get_user_class(reply_socket).get_name()))
		ft_send(reply_socket, ERR_NOTONCHANNEL + server.get_user_class(reply_socket).get_name() + " " + channel + " :You aren't on that channel\r\n");
	else if (server.is_on_channel(channel, username))
		ft_send(reply_socket, ERR_USERONCHANNEL + server.get_user_class(reply_socket).get_name() + " " + username + " " + channel + " :is already on channel\r\n");
	else {
		User& user = server.get_user_class(username);
		ft_send(user.get_socketfd(), ":" + server.get_user_class(reply_socket).get_name() + " INVITE " + username + " " + channel + "\r\n");
		server.get_channel_class(channel).add_invited(username);
		ft_send(reply_socket, RPL_INVITING + 
			server.get_user_class(reply_socket).get_name() + " " + username + " " + channel + "\r\n");
	}
}

void	kick_command( Server& server, int reply_socket, std::istringstream &message ) {
	std::string channel;
	std::string users_str;
	std::string user;
	std::string kick_message;

	std::getline(message, channel, ' ');
	std::getline(message, users_str, ' ');
	std::istringstream users(users_str);
	std::getline(message, kick_message, ':');
	if (!std::getline(message, kick_message))
		kick_message = "kicked from channel by " + server.get_user_class(reply_socket).get_name();

	if (users_str.empty() || channel.empty())
		throw NeedMoreParamsException();
	if (!(server.get_channel_class(channel)).is_op(server.get_user_class(reply_socket).get_name()))
		throw ChanOPrivsNeededException();
	if (!server.is_on_channel(channel, server.get_user_class(reply_socket).get_name()))
		throw NotOnChannelException();
	while (std::getline(users, user, ',')) {
		if (!server.is_on_channel(channel, user)) //user not in channel
			ft_send(reply_socket, "441 " + user + " " + channel + ":They Aren't on that channel\r\n");
		else {
			server.get_channel_class(channel).send_channel(":" + 
			server.get_user_class(reply_socket).get_name()  + "!" + server.get_user_class(reply_socket).get_realname() + "@" + 
				server.get_user_class(reply_socket).get_ip() + " KICK " + 
				channel + " " + user + " :" + kick_message + "\r\n");
			server.get_channel_class(channel).user_kicked(server.get_user_class(reply_socket), server.get_user_class(user), kick_message);
	
			if (!server.get_channel_class(channel).get_size()) {
				Channel& chan = server.get_channel_class(channel);
				server.get_channels_list().erase(std::remove(server.get_channels_list().begin(), server.get_channels_list().end(), &chan), server.get_channels_list().end());
				delete &chan;
			}
		}
	}
}

void	part_command( Server& server, int reply_socket, std::istringstream &message ) {
	std::string	channel;
	std::string part_msg;
	
	std::getline(message, channel, ' ');
	std::getline(message, part_msg, ':');
	std::getline(message, part_msg);

	if (!server.is_on_channel(channel, server.get_user_class(reply_socket).get_name()))
		return ; //! NOT ON CHANNEL
	if (channel.empty())
		return ; //!NEEDMOREPARAMS
	server.part_channel(server.get_user_class(reply_socket).get_name(), channel , part_msg);
	server.get_user_class(reply_socket).remove_channel_list(&server.get_channel_class(channel));
	if (!server.get_channel_class(channel).get_size()) {
		Channel& chan = server.get_channel_class(channel);
		server.get_channels_list().erase(std::remove(server.get_channels_list().begin(), server.get_channels_list().end(), &chan), server.get_channels_list().end());
		delete &chan;
	}
}

void	quit_command( Server& server, int reply_socket, std::istringstream &message ) {	
	try
	{
		User& user = server.get_user_class(reply_socket);
		std::vector<Channel*> channel_list = user.get_list_channel();

		for (size_t j = 0; j < channel_list.size(); j++){
			channel_list[j]->user_quit(user, message.str() + "\r\n");
			if (!channel_list[j]->get_size()) {
				Channel& chan = server.get_channel_class(channel_list[j]->get_name());
				server.get_channels_list().erase(std::remove(server.get_channels_list().begin(), server.get_channels_list().end(), &chan), server.get_channels_list().end());
				delete &chan;
			}

		}
		
		server.get_connected_user().erase(std::remove(server.get_connected_user().begin(), server.get_connected_user().end(), 
			&user), server.get_connected_user().end());
		
		delete &user;
	}
	catch(const std::exception& e) {}
	server.remove_poll_fd(reply_socket);
}

void pass_command(Server &server, int reply_socket, std::istringstream &message) {
	try
	{
		if (server.get_user_class(reply_socket).user_authenticate()) {
			ft_send(reply_socket, "462 " + server.get_user_class(reply_socket).get_name() + " :You may not reregister\r\n");
			return ;
		}
	}
	catch(const std::exception& e) {}
	
	if (message.str() != server.get_pass() && server.get_pass() != ""){
		try 	//remove user object
		{
			User& user = server.get_user_class(reply_socket);
			ft_send(reply_socket, "464 " + user.get_name() + " :Password incorrect\r\n");

			server.get_connected_user().erase(std::remove(server.get_connected_user().begin(), server.get_connected_user().end(), 
			&user), server.get_connected_user().end());

			delete(&user);
		}
		catch(const std::exception& e) {}
		server.remove_poll_fd(reply_socket);
	} else {
		try
		{
			server.get_user_class(reply_socket).set_password(message.str());
		}
		catch(const std::exception& e) {}
		
	}
}

void	shutdown_command(Server& server) {
	server.~Server();
	exit(0);
}
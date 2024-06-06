#include "Channel.hpp"
#include "error_numeric.hpp"
#include "ft_irc.hpp"
#include "numeric_replies.hpp"
#include <cstddef>
#include <sstream>
#include <string>
#include "Server.hpp"
#include <ctime>
#include <vector>

void	pong(int reply_socket, std::string message) {
	ft_send(reply_socket, "PONG " + message + "\n");
}

void	motd_command( Server& server, int reply_socket ) {
	if (server.get_motd().empty())
		ft_send(reply_socket, "422 " + server.get_user_class(reply_socket).get_name() + " :No MOTD set");
	else
		ft_send(reply_socket, "375 " + server.get_user_class(reply_socket).get_name() + " :Message of the Day \n372 :" + 
				server.get_motd() + "\n376 " + server.get_user_class(reply_socket).get_name() + " :End of MOTD.");
}

void	version_command( int reply_socket ) {
	ft_send(reply_socket, RPL_VERSION);
	ft_send(reply_socket, RPL_ISUPPORT);
}

void	nick_command( Server& server, int reply_socket, std::string message ) {
	//checks needed here change if not already taken by SOMEONE ELSE
	std::string oldnick = server.get_user_class(reply_socket).get_name();

	server.change_nick(server.get_user_class(reply_socket), message);
	server.send_all(":" + oldnick + " NICK " + server.get_user_class(reply_socket).get_name() + "\n");
}

void	cap_command( Server& server, int reply_socket, std::istringstream &message ) {
	(void) server;

	return ;
	std::string	param;
	std::getline(message, param, ' ');
	if (param == "LS")
		ft_send(reply_socket, "CAP * LS :");
	else if (param == "LIST")
		ft_send(reply_socket, "CAP * LIST :");
}

void	join_command( Server& server, int reply_socket, std::istringstream &message ) {
	server.join_channel(server.get_user_class(reply_socket).get_name(), 
						message.str());
	server.get_user_class(reply_socket).add_channel_list(&server.get_channel_class(message.str()));
	if (server.get_channel_class(message.str()).get_topic().empty())
		no_topic_set(reply_socket, message.str());
	else
		topic_command(server, reply_socket, message);
}

void	privmsg_command( Server& server, int reply_socket, std::istringstream &message ) {

	if (find(message.str().begin(), message.str().end(), ':') == message.str().end()) {	//detection de l'absence de cible
		ft_send(reply_socket, "411 " + server.get_user_class(reply_socket).get_name() + " :No recipient given (PRIVMSG " + message.str() + " )\n");
		return ;
	}

	std::string recipients_list;
	std::getline(message, recipients_list, ':');
	std::string msg;
	std::getline(message, msg);
	if (!msg.size()) {
		ft_send(reply_socket, "412 " + server.get_user_class(reply_socket).get_name() + " :No text to send\n");
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
			std::cout << recipient << " is a user\n";
			ft_send(user.get_socketfd(), ":" + server.get_user_class(reply_socket).get_name() + " PRIVMSG " + recipient + " :" + msg + "\n");
		}
		catch(const std::exception& e)	//ce n'est pas un user
		{
			try
			{
				Channel channel = server.get_channel_class(recipient);
				std::cout << recipient << " is a channel\n";
					channel.send_channel(reply_socket, ":" + server.get_user_class(reply_socket).get_name() + " PRIVMSG " + recipient + " :" + msg + "\n");
			}
			catch(const std::exception& e) //ce n'est pas un channel non plus
			{
				ft_send(reply_socket, "401 " + server.get_user_class(reply_socket).get_name() + " " + recipient + " :No such nick/channel");
			}
		}
	}
	
}

//TODO EMPTY
void	mode_command( Server& server, int reply_socket, std::istringstream &message ) {
	return;

	std::string target;
	std::string value;
	bool		op_sign;
	t_enum_modes mode;

	std::getline(message, target, ' ');
	std::getline(message, value, ' ');
	if (value == "+")
		op_sign = true;
	
	int size = std::strtol(value.c_str(),NULL, NULL);
/*    Command: MODE
		Parameters: <target> [<modestring> [<mode arguments>...]]*/
//?Target is channel or user?
	//?if channel param is password?
		//yes
	std::string password;
	std::getline(message, password, ' ');

	server.get_channel_class(message.str()).set_mode(KEY, op_sign, password);
//if not password
	switch (mode)
	{
	case INVITE:
		server.get_channel_class(message.str()).set_mode( INVITE, size ) ;;
		break ;
	case TOPIC:
		server.get_channel_class(message.str()).set_mode( TOPIC, size ) ;
		break ;
	case LIMIT:
		server.get_channel_class(message.str()).set_mode( LIMIT, size ) ;
		break ;
	default:
		break;
	}
//if user
	server.get_channel_class(message.str()).set_mode( OP,  server.get_user_class(reply_socket), target, boolean ) ;


}

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
	ft_send(reply_socket, ERR_NEEDMOREPARAMS + user + "TOPIC :Not enough parameters");

try {
	if (server.is_on_channel(channel, user)) {
		if (server.is_op(channel, user) || server.get_channel_class(channel).topic_mode_is_off() == true || topic_message.empty()) {
			if (topic_message.empty()) {
				if (server.get_topic(channel).empty())
					no_topic_set(reply_socket, message.str());
				else {
					msg_to_send << RPL_TOPIC << server.get_user_class(reply_socket).get_name() << channel << "\n";
					if (ft_send(reply_socket, msg_to_send.str()) == -1)
						std::cerr << "[Server] Send error to client " << server.get_user_class(reply_socket).get_name() << ": " <<  strerror(errno)  << std::endl;
				}
			}
			else {

				msg_to_send << RPL_TOPIC << server.get_user_class(reply_socket).get_name() << " " << channel << " :" << topic_message << "\n";
				server.send_channel(channel, reply_socket, msg_to_send.str());
				if (ft_send(reply_socket, msg_to_send.str()) == -1)
					std::cerr << "[Server] Send error to client " << server.get_user_class(reply_socket).get_name() << ": " <<  strerror(errno)  << std::endl;
			}
		}
		else {
			if (!server.is_op(channel, user)) {
				msg_to_send << ERR_CHANOPRIVSNEEDED << user << " " << channel << " :You're not channel operator\n";
				if (ft_send(reply_socket, msg_to_send.str()) == -1)
					std::cerr << "[Server] Send error to client " << server.get_user_class(reply_socket).get_name() << ": " <<  strerror(errno)  << std::endl;
			}
		}
	}
	else
		ft_send(reply_socket, "442 " + user + " " + channel + "You are not on channel " + channel + "\n");
	}
catch(const NoSuchChannelException& e) {
	ft_send(reply_socket, ERR_NOSUCHCHANNEL + user + " " + channel + " :No such channel " + channel + "\n");
}
}

//NAMES => list all channel and their occupant, then all users outside any channel, under the "channel *"
//NAMES #CHAN1,#CHAN2 => list all users on channel(s)
void	names_command( Server& server, int reply_socket, std::istringstream &message ) {
	//names
	(void) server;
	(void) reply_socket;
	(void) message;


	int i = 0;	// le nombre de channel passés en parametre
	User user = server.get_user_class(reply_socket);

	for (std::string channel_name; std::getline(message, channel_name, ',');i++) {
		try
		{
			server.get_channel_class(channel_name).send_userlist(user);
		}
		catch(const std::exception& e)
		{
			ft_send(reply_socket, "366 " + user.get_name() + " " + channel_name + " :End of /NAMES list/n");
		}
	}
	if (!i ) //aucuns parametres
	{
		std::vector<Channel*> channel_list = server.get_channels_list();
		for (int j = 0; channel_list[j] != channel_list.back(); j++)
		{
			channel_list[j]->send_userlist(user);
		}
		ft_send(reply_socket, "353 " + user.get_name() + " * :");
		std::vector<User*> user_list = server.get_connected_user();
		for (int j = 0; user_list[j] < user_list.back(); j++)
		{
			if (!user_list[j]->get_list_channel().size())
				ft_send(reply_socket, user_list[j]->get_name());
		}
		ft_send(reply_socket, "366 " + user.get_name() + " * :End of /NAMES list/n");
	}
}

void	list_command( Server& server, int reply_socket, std::istringstream &message ) {
	(void) server;
	(void) reply_socket;
	(void) message;

	if (!message.str().empty()) 
		return ;
	const User& user = server.get_user_class(reply_socket);
	const std::vector<Channel *> chans = server.get_channels_list();
	const size_t len = chans.size();
	for (size_t i = 0; i < len; i++) {
		ft_send(reply_socket, RPL_LIST + user.get_name() + " " + chans[i]->get_name() + " " +
		chans[i]->user_count() + " :" + chans[i]->get_topic() + "\n");
	}
	ft_send(reply_socket, RPL_LISTEND + user.get_name() + " :End of /LIST\n");
}

void	whois_command( Server& server, int reply_socket, std::istringstream &message ) {
	const User& user = server.get_user_class(reply_socket);
	std::string target;

	if (message.str().empty()) {
		ft_send(reply_socket, ERR_NONICKNAMEGIVEN + user.get_name() + " :No nickname given\n");
		return ;
	}
	
	std::getline(message, target);
	try {
		ft_send(reply_socket, RPL_WHOISUSER + user.get_name() + " " + target + " " + target + " " +
			server.get_user_class(target).get_ip() + " * :" + server.get_user_class(target).get_realname() + "\n"); // whoisUSER
		const size_t len = server.get_user_class(target).get_list_channel().size();
		User& target_u = server.get_user_class(target);
		const std::vector<Channel *> chans = target_u.get_list_channel();
		std::string whois_channel_string = RPL_WHOISCHANNELS + user.get_name() + " " + target + " :" ;
		for (size_t i = 0; i < len; i++) {
			if (chans[i]->is_op(target_u))
				whois_channel_string += "&";
			whois_channel_string += chans[i]->get_name() + " ";
		}
		ft_send(reply_socket, whois_channel_string + "\n");
		ft_send(reply_socket, RPL_ENDOFWHOIS + user.get_name() + " " + target + " :End of /WHOIS list\n");
	}
	catch(const NoSuchNickException& e) {
		ft_send(reply_socket, ERR_NOSUCHNICK + user.get_name() + " " + target + " :No such nick\n");
		return ;
	}
}

//TODO How to answer an invite???
void	invite_command( Server& server, int reply_socket, std::istringstream &message ) {
	(void) server;
	(void) reply_socket;
	(void) message;

	std::string user;
	std::string channel;

	if (message.str().empty()) {
		ft_send(reply_socket, ERR_NEEDMOREPARAMS + server.get_user_class(reply_socket).get_name() + " INVITE :Not enough parameters\n");
		return ;
	}
	std::getline(message, user);
	std::getline(message, channel);
	if (!server.is_on_channel(channel, server.get_user_class(reply_socket).get_name()))
		ft_send(reply_socket, ERR_NOTONCHANNEL + server.get_user_class(reply_socket).get_name() + " " + channel + " :You aren't on that channel\n");
	else if (server.is_on_channel(channel, user))
		ft_send(reply_socket, ERR_USERONCHANNEL + server.get_user_class(reply_socket).get_name() + " " + user + " " + channel + " :is already on channel\n");
	else {
		ft_send(reply_socket, RPL_INVITING + server.get_user_class(reply_socket).get_name() + " " + channel + "\n");
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
	if (message.str().empty())
		kick_message = "kicked from channel by " + server.get_user_class(reply_socket).get_name();
	else
		kick_message = message.str();

	if (users_str.empty())
		throw NeedMoreParamsException();
	if (!(server.get_channel_class(channel)).is_op(server.get_user_class(reply_socket).get_name()))
		throw ChanOPrivsNeededException();
	if (!server.is_on_channel(channel, server.get_user_class(reply_socket).get_name()))
		throw NotOnChannelException();
	while (std::getline(users, user, ',')) {
		if (!server.is_on_channel(channel, user)) //user not in channel
			ft_send(reply_socket, "441 " + user + " " + channel + ":They Aren't on that channel");
		else {
			server.get_channel_class(channel).send_channel(reply_socket, "NOTICE " + channel + kick_message);
		}
	}
}

void	part_command( Server& server, int reply_socket, std::istringstream &message ) {
	std::string	channel;

	std::getline(message, channel, ' ');

	server.part_channel(server.get_user_class(reply_socket).get_name(), channel , message.str());
	server.get_user_class(reply_socket).remove_channel_list(&server.get_channel_class(message.str()));
}
//TODO EMPTY
void	quit_command( Server& server, int reply_socket, std::istringstream &message ) {
	try
	{
		User user = server.get_user_class(reply_socket);
		std::vector<Channel*> channel_list = user.get_list_channel();
		for (size_t j = 0; j < channel_list.size(); j++)
			channel_list[j]->user_quit(user, message.str() + "\n");
		const size_t len = server.get_connected_user().size();
			for (size_t i = 0; i < len; i++) {
				if (server.get_connected_user()[i] == &user) {
					server.get_connected_user().erase(server.get_connected_user().begin() + i);
					break ;
				}
		}
		//delete(&user);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << "\n";
	}
	server.remove_poll_fd(reply_socket);
}

void pass_command(Server &server, int reply_socket, std::istringstream &message)
{
	if (message.str() != server.get_pass()){
		ft_send(reply_socket, "Wrong password\n");
		try 	//remove user object
		{
			User user = server.get_user_class(reply_socket);
			const size_t len = server.get_connected_user().size();
				for (size_t i = 0; i < len; i++) {
					if (server.get_connected_user()[i] == &user) {
						server.get_connected_user().erase(server.get_connected_user().begin() + i);
						break ;
					}
			}
			//delete(&user);
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
		server.remove_poll_fd(reply_socket);
	}
}
#include "ft_irc.hpp"
#include "numeric_replies.hpp"
#include <sstream>
#include <string>
#include "Server.hpp"
#include <ctime>

void	pong(int reply_socket, std::string message) {
	ft_send(reply_socket, "PONG " + message.substr(5));
}

void	motd_command( Server& server, int reply_socket ) {
	if (server.get_motd().empty())
		ft_send(reply_socket, "422 :No MOTD set");
	else
		ft_send(reply_socket, "375 :Message of the Day \n372 :" + 
				server.get_motd() + "\n376 :End of MOTD.");
}

void	version_command( int reply_socket ) {
	ft_send(reply_socket, RPL_VERSION);
	ft_send(reply_socket, RPL_ISUPPORT);
}

void	nick_command( Server& server, int reply_socket, std::string message ) {
	//checks needed here change if not already taken by SOMEONE ELSE
	server.change_nick(server.get_user_class(reply_socket), message);
}

void	cap_command( Server& server, int reply_socket, std::istringstream &message ) {
	(void) server;
	std::string	param;
	std::getline(message, param, ' ');
	if (param == "LS") {
		ft_send(reply_socket, "CAP * LS: ");
	}
}

void	join_command( Server& server, int reply_socket, std::istringstream &message ) {
	server.join_channel(server.get_user_class(reply_socket).get_name(), 
						message.str());
}

void	privmsg_command( Server& server, int reply_socket, std::istringstream &message ) {

	if (find(message.str().begin(), message.str().end(), ':') == message.str().end()) {	//detection de l'absence de cible
		ft_send(reply_socket, "411 : No recipient given (PRIVMSG " + message.str() + " )");
		return ;
	}

	std::string recipients_list;
	std::getline(message, recipients_list, ':');
	std::string msg;
	std::getline(message, msg);
	if (!msg.size()) {
		ft_send(reply_socket, "412 : No text to send");
		return ;
	}

	std::istringstream stream;
	stream.str(recipients_list);
	for(std::string recipient; std::getline(stream, recipient, ',');) {
		if (recipient[recipient.size() - 1] == ' ')
			recipient.erase(recipient.end() - 1); //-1 pour enlever l'espace
		std::cout << "recipient name: \"" << recipient << "\"\n";
		try
		{
			User user = server.get_user_class(recipient);
			std::cout << recipient << " is a user\n";
			ft_send(user.get_socketfd(), msg);
		}
		catch(const std::exception& e)	//ce n'est pas un user
		{
			std::cout << recipient << " isn't a user\n";
			try
			{
				Channel channel = server.get_channel_class(recipient);
				std::cout << recipient << " is a channel\n";
				channel.send_channel(reply_socket, msg);
			}
			catch(const std::exception& e) //ce n'est pas un channel non plus
			{
				std::cout << recipient << " isn't a channel\n";
				ft_send(reply_socket, "401 : " + recipient + " :No such nick/channel");
			}
		}
	}
	
}

void	part_command( Server& server, int reply_socket, std::istringstream &message ) {
	std::string	channel;

	std::getline(message, channel, ' ');
	server.part_channel(server.get_user_class(reply_socket).get_name(), channel , message.str());
}

void	topic_command( Server& server, int reply_socket, std::istringstream &message ) {
	// std::string	channel;
	// std::string topic_message;
	// std::string topic_whotime;
	// std::getline(message, channel, ' ');
	// std::string user = server.get_user_class(reply_socket).get_name();
	// std::time_t result = std::time(NULL);

	// if (server.is_op(channel, user))
	// 	if (server.is_on_channel(channel, user)) {
	// 		if (message.str().empty()) {
	// 			if (server.get_topic(channel).empty()) {
	// 				std::cout << "No topic set for"  << channel << std::endl;
	// 			}
	// 			else {
	// 				std::cout << server.get_topic(channel) << std::endl;
	// 			}
	// 			if (RPL_TOPICWHOTIME) {
	// 				std::cout << "Topic set by " << RPL_TOPICWHOTIME << std::endl;
	// 			}
	// 		}
	// 		else {
	// 			topic_whotime = RPL_TOPICWHOTIME + user + std::asctime(std::localtime(&result));
	// 			server.set_topic(channel, message.str(), topic_whotime);
	// 		}

	// }
	// else if (server.is_op(channel, user)) {
	// 	if (server.is_on_channel(channel, user)) {

	// 	}
		
	// }




	// TOPIC CHANNEL topic si change sinon sans
	(void) server;
	(void) reply_socket;
	(void) message;

}

//NAMES => list all channel and their occupant, then all users outside any channel, under the "channel *"
//NAMES #CHAN1,#CHAN2 => list all users on channel(s)
void	names_command( Server& server, int reply_socket, std::istringstream &message ) {
	//names
	(void) server;
	(void) reply_socket;
	(void) message;

	int i = 0;	// le nombre de channel passés en parametre
	User user(server.get_user_class(reply_socket));

	ft_send(reply_socket, "353 ");
	for (std::string channel_name; std::getline(message, channel_name, ',');i++) {
		try
		{
			server.get_channel_class(channel_name).send_userlist(user);
			
		}
		catch(const std::exception& e)
		{
			//do_nothing
		}
		ft_send(reply_socket, channel_name);
	}
	if (!i ) //aucuns parametres
	{
		std::vector<Channel*> channel_list = server.get_channels_list();
		for (int j = 0; channel_list[j] != channel_list.back(); j++)
		{
			ft_send(reply_socket, channel_list[j]->get_name());
			channel_list[j]->send_userlist(user);
		}
		ft_send(reply_socket, "*:\n");
		std::vector<User*> user_list = server.get_connected_user();
		for (int j = 0; user_list[j] < user_list.back(); j++)
		{
			if (!user_list[j]->get_list_channel().size())
				ft_send(reply_socket, user_list[j]->get_name());
		}
	}
	ft_send(reply_socket, "366 ");
}

//Liste tous les canaux 
void	list_command( Server& server, int reply_socket, std::istringstream &message ) {
	(void) server;
	(void) reply_socket;
	(void) message;
}

void	invite_command( Server& server, int reply_socket, std::istringstream &message ) {
	(void) server;
	(void) reply_socket;
	(void) message;

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

void	quit_command( Server& server, int reply_socket, std::istringstream &message ) {
	(void) server;
	(void) reply_socket;
	(void) message;
}

#include "ft_irc.hpp"
#include "numeric_replies.hpp"
#include <sstream>
#include <string>
#include "Server.hpp"

void	pong(int reply_socket, std::string message) {
	std::cout << "PING\n";
	ft_send(reply_socket, "PONG " + message.substr(5));
}

void	motd_command( Server server, int reply_socket ) {
	std::cout << "MOTD\n";
	if (server.get_motd().empty())
		ft_send(reply_socket, "422 :No MOTD set");
	else
		ft_send(reply_socket, "375 :Message of the Day \n372 :" + 
				server.get_motd() + "\n376 :End of MOTD.");
}

void	version_command( int reply_socket ) {
	std::cout << "Version\n";
	ft_send(reply_socket, RPL_VERSION);
	ft_send(reply_socket, RPL_ISUPPORT);
}

void	nick_command( Server server, std::string message ) {
	std::cout << "NICK\n";
	server.change_nick(message.substr(5));
}

void	join_command( Server server, int reply_socket, std::istringstream &message ) {
	std::cout << "JOIN\n";
	server.join_channel(server.find_user_from_fd(reply_socket)->get_name(), 
						message.str());
}

void	part_command( Server server, int reply_socket, std::istringstream &message ) {
	std::string	channel;

	std::cout << "PART\n";
	std::getline(message, channel, ' ');
	server.part_channel(server.find_user_from_fd(reply_socket)->get_name(), channel , message.str());
}

void	topic_command( Server server, int reply_socket, std::istringstream &message ) {
	// TOPIC CHANNEL topic si change sinon sans
	(void) server;
	(void) reply_socket;
	(void) message;

	std::cout << "TOPIC\n";
}

//NAMES => list all channel and their occupant, then all users outside any channel, under the "channel *"
//NAMES #CHAN1,#CHAN2 => list all users on channel(s)
void	names_command( Server server, int reply_socket, std::istringstream &message ) {
	//names
	(void) server;
	(void) reply_socket;
	(void) message;

	std::cout << "NAMES\n";
	int i = 0;
	for (std::string channel_name; std::getline(message, channel_name, ' ');i++) {
		Channel channel = server._get_channel_class(channel_name);

		send(reply_socket, channel_name.c_str(), channel_name.size(), 0);
	}
	if (!message.str().c_str()[0])	//pas d'arguments
		;
}

//Liste tous les canaux 
void	list_command( Server server, int reply_socket, std::istringstream &message ) {
	(void) server;
	(void) reply_socket;
	(void) message;
	std::cout << "LIST\n";
}

void	invite_command( Server server, int reply_socket, std::istringstream &message ) {
	(void) server;
	(void) reply_socket;
	(void) message;
	std::cout << "INVITE\n";

}

void	kick_command( Server server, int reply_socket, std::istringstream &message ) {
	std::string channel;
	std::string users_str;
	std::string user;
	std::string kick_message;

	std::cout << "KICK\n";
	std::getline(message, channel, ' ');
	std::getline(message, users_str, ' ');
	std::istringstream users(users_str);
	if (message.str().empty())
		kick_message = "kicked from channel by " + server.find_user_from_fd(reply_socket)->get_name();
	else
		kick_message = message.str();

	if (users_str.empty())
		throw NeedMoreParamsException();
	if (!(server._get_channel_class(channel))._is_op(server.find_user_from_fd(reply_socket)->get_name()))
		throw ChanOPrivsNeededException();
	if (!server.is_on_channel(channel, server.find_user_from_fd(reply_socket)->get_name()))
		throw NotOnChannelException();
	while (std::getline(users, user, ',')) {
		if (!server.is_on_channel(channel, user)) //user not in channel
			ft_send(reply_socket, "441 " + user + " " + channel + ":They Aren't on that channel");
		else {
			server._get_channel_class(channel).send_channel("NOTICE " + channel + kick_message);
		}
	}
}

void	quit_command( Server server, int reply_socket, std::istringstream &message ) {
	(void) server;
	(void) reply_socket;
	(void) message;
	std::cout << "QUIT\n";
}

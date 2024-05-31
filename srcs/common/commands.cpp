#include "ft_irc.hpp"
#include "numeric_replies.hpp"
#include <cstdio>
#include <sstream>
#include <string>

void	pong(Server server, int reply_socket, std::string message) {
	ft_send(reply_socket, "PONG " + message.substr(5));
}

void	motd_command( Server server, int reply_socket, std::string message ) {
	ft_send(reply_socket, "MOTD " + server.get_motd());
}

void	version_command( Server server, int reply_socket, std::string message ) {
	ft_send(reply_socket, RPL_VERSION);
	ft_send(reply_socket, RPL_ISUPPORT);
}

void	nick_command( Server server, int reply_socket, std::string message ) {
	server.change_nick(message.substr(5));
}

void	join_command( Server server, int reply_socket, std::string message ) {
	server.join_channel(server.find_user_from_fd(reply_socket)->get_name(), message);
}

void	part_command( Server server, int reply_socket, std::string message ) {
	std::istringstream msg(message);
	std::string	channel;
	std::string	part_message;

	std::getline(msg, channel, ' ');
	std::getline(msg, part_message);
	server.part_channel(server.find_user_from_fd(reply_socket)->get_name(), channel , part_message);
}

void	topic_command( Server server, int reply_socket, std::string message ) {
	// TOPIC CHANNEL topic si change sinon sans
}

//NAMES => list all visible users
//NAMES #CHAN1,#CHAN2 => list all on channel(s)
void	names_command( Server server, int reply_socket, std::string message ) {
	//names 
}

//Liste tous les canaux 
void	list_command( Server server, int reply_socket, std::string message ) {

}

void	invite_command( Server server, int reply_socket, std::string message ) {

}

void	kick_command( Server server, int reply_socket, std::string message ) {

}

void	whois_command( Server server, int reply_socket, std::string message ) {

}

void	quit_command( Server server, int reply_socket, std::string message ) {

}

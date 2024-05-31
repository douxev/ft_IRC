#include "ft_irc.hpp"
#include "numeric_replies.hpp"

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
	
}

void	part_command( Server server, int reply_socket, std::istringstream &message ) {
	std::string	channel;

	std::getline(message, channel, ' ');
	server.part_channel(server.find_user_from_fd(reply_socket)->get_name(), channel , message.str());
}

void	topic_command( Server server, int reply_socket, std::istringstream &message ) {
	// TOPIC CHANNEL topic si change sinon sans
}

//NAMES => list all visible users
//NAMES #CHAN1,#CHAN2 => list all on channel(s)
void	names_command( Server server, int reply_socket, std::istringstream &message ) {
	//names 
}

//Liste tous les canaux 
void	list_command( Server server, int reply_socket, std::istringstream &message ) {

}

void	invite_command( Server server, int reply_socket, std::istringstream &message ) {

}

void	kick_command( Server server, int reply_socket, std::istringstream &message ) {

}

void	whois_command( Server server, int reply_socket, std::istringstream &message ) {

}

void	quit_command( Server server, int reply_socket, std::istringstream &message ) {

}

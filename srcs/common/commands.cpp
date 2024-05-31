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

void	part_command( Server server, int reply_socket, std::string message ) {

}

void	topic_command( Server server, int reply_socket, std::string message ) {

}

void	names_command( Server server, int reply_socket, std::string message ) {

}

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

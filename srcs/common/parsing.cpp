#include "ft_irc.hpp"

static std::string	first_word( std::string cmd ) {

}

void	parse_commands( Server server, int reply_socket, std::string message ) {

	const std::string cmd = first_word(message);

	if (cmd == "PING")
		ft_send(reply_socket, "PONG " + message.substr(5, message.size() - 5));
	else if (cmd == "MOTD")
		;
	else if (cmd == "VERSION")
		;
	else if (cmd == "NICK")
		;
	else if (cmd == "JOIN")
		;
	else if (cmd == "PART")
		;
	else if (cmd == "TOPIC")
		;
	else if (cmd == "NAMES")
		;
	else if (cmd == "LIST")
		;
	else if (cmd == "INVITE")
		;
	else if (cmd == "KICK")
		;
	else if (cmd == "QUIT")
		;
}

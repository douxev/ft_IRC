#include "ft_irc.hpp"

static std::string	get_command( std::string cmd ) {
	
}

void	parse_commands( Server server, int reply_socket, std::string message ) {

	const std::string cmd = get_command(message);

	if (cmd == "USER")
		init_client(server, reply_socket, message);
	else if (cmd == "PING")
		pong(server, reply_socket, message);
	else if (cmd == "MOTD")
		motd_command(server, reply_socket, message);
	else if (cmd == "VERSION")
		version_command(server, reply_socket, message);
	else if (cmd == "NICK")
		nick_command(server, reply_socket, message);
	else if (cmd == "JOIN")
		join_command(server, reply_socket, message);
	else if (cmd == "PART")
		part_command(server, reply_socket, message);
	else if (cmd == "TOPIC")
		topic_command(server, reply_socket, message);
	else if (cmd == "NAMES")
		names_command(server, reply_socket, message);
	else if (cmd == "LIST")
		list_command(server, reply_socket, message);
	else if (cmd == "INVITE")
		invite_command(server, reply_socket, message);
	else if (cmd == "KICK")
		kick_command(server, reply_socket, message);
	else if (cmd == "WHOIS")
		whois_command(server, reply_socket, message);
	else if (cmd == "QUIT")
		quit_command(server, reply_socket, message);
}

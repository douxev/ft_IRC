#include "ft_irc.hpp"
<<<<<<< HEAD
#include <exception>
=======
#include "numeric_replies.hpp"
>>>>>>> refs/remotes/origin/main
#include <ctime>

static std::string	get_command( std::string cmd ) {
	
}

void	init_client( Server server, int reply_socket, std::string message) {
	std::stringstream msg_to_sent;
	std::time_t result = std::time(nullptr);
	msg_to_sent << RPL_WELCOME << "Welcome to the GuiRaMa Internet Relay Chat Network " << reply_socket << std::endl;
	msg_to_sent << RPL_YOURHOST << "You host is irc.guirama.42, running on version " << RPL_VERSION << std::endl;
	msg_to_sent << RPL_CREATED << "this server was created "<< std::asctime(std::localtime(&result)) << std::endl;
	msg_to_sent << RPL_MYNFO << "irc.guirama.42 " << RPL_VERSION << ""/*<available user modes><available channel modes> [<channel modes with a parameter>]*/ << std::endl;
	msg_to_sent << RPL_ISUPPORT << ""/* tous les parametres qu'on utilisera pour ISUPPORT */ << std::endl;
	// if (send(reply_socket, msg_to_sent.str().c_str(), msg_to_sent.str().size(), 0) == -1)
	// 	std::cerr << "[Server] Send error to client " << reply_socket << ": " <<  strerror(errno)  << std::endl;
}

//ADD TRY CATCH EXCEPTIONS FOR SENDING GOOD REPLIES
void	parse_commands( Server server, int reply_socket, std::string message ) {

	const std::string cmd = get_command(message);

	try {

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
	catch (std::exception e) {
		std::cout << e.what() << std::endl;
	}
}

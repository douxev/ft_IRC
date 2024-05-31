#include "ft_irc.hpp"
#include <cstddef>
#include <exception>
#include "numeric_replies.hpp"
#include <ctime>
#include <sstream>
#include <string>

void	init_client( Server& server, int reply_socket, std::string message) {

	(void) server;
	(void) message;
	std::stringstream msg_to_send;
	std::time_t result = std::time(NULL);
	
	//Parsing de message
	std::cout << "USER" << std::endl;
	msg_to_send.str("");
	msg_to_send << RPL_WELCOME << server.find_user_from_fd(reply_socket)->get_name() << "\n";
	if (ft_send(reply_socket, msg_to_send.str()) == -1)
		std::cerr << "[Server] Send error to client " << server.find_user_from_fd(reply_socket)->get_name() << ": " <<  strerror(errno)  << std::endl;
	msg_to_send.str("");
	msg_to_send << RPL_YOURHOST << "\n";
	if (ft_send(reply_socket, msg_to_send.str()) == -1)
		std::cerr << "[Server] Send error to client " << server.find_user_from_fd(reply_socket)->get_name() << ": " <<  strerror(errno)  << std::endl;
	msg_to_send.str("");
	msg_to_send << RPL_CREATED << std::asctime(std::localtime(&result)) << "\n";
	if (ft_send(reply_socket, msg_to_send.str()) == -1)
		std::cerr << "[Server] Send error to client " << server.find_user_from_fd(reply_socket)->get_name() << ": " <<  strerror(errno)  << std::endl;
	msg_to_send.str("");
	msg_to_send << RPL_MYNFO << "\n"/*<available user modes><available channel modes> [<channel modes with a parameter>]*/;
	if (ft_send(reply_socket, msg_to_send.str()) == -1)
		std::cerr << "[Server] Send error to client " << server.find_user_from_fd(reply_socket)->get_name() << ": " <<  strerror(errno)  << std::endl;
	msg_to_send.str("");
	msg_to_send << RPL_ISUPPORT << "\n"/* tous les parametres qu'on utilisera pour ISUPPORT */;
	if (ft_send(reply_socket, msg_to_send.str()) == -1)
		std::cerr << "[Server] Send error to client " << server.find_user_from_fd(reply_socket)->get_name() << ": " <<  strerror(errno)  << std::endl;

}

//ADD TRY CATCH EXCEPTIONS FOR SENDING GOOD REPLIES
void	parse_commands( Server& server, int reply_socket, std::istringstream& message ) {

	std::string cmd;
	std::string line_str;

	std::vector<std::string> rev_lines;
	std::string rline;
	while (std::getline(message, rline))
	{
		// Store the lines in reverse order.
		rev_lines.insert(rev_lines.begin(), rline);
	}


	
	// while (std::getline(message, line_str)) {
	for (size_t i = 0; i < rev_lines.size(); i++) {

	line_str = rev_lines[i];
	std::istringstream line(line_str);
	std::getline(line, cmd, ' ');
	try {

		if (cmd == "USER")
			init_client(server, reply_socket, line.str());
		else if (cmd == "PING")
			pong(reply_socket, line.str());
		else if (cmd == "MOTD")
			motd_command(server, reply_socket);
		else if (cmd == "VERSION")
			version_command(reply_socket);
		else if (cmd == "NICK")
			nick_command(server, reply_socket, line.str());
		else if (cmd == "CAP")
			cap_command(server, reply_socket, line);
		else if (cmd == "JOIN")
			join_command(server, reply_socket, line);
		else if (cmd == "PART")
			part_command(server, reply_socket, line);
		else if (cmd == "TOPIC")
			topic_command(server, reply_socket, line);
		else if (cmd == "NAMES")
			names_command(server, reply_socket, line);
		else if (cmd == "LIST")
			list_command(server, reply_socket, line);
		else if (cmd == "INVITE")
			invite_command(server, reply_socket, line);
		else if (cmd == "KICK")
			kick_command(server, reply_socket, line);
		else if (cmd == "QUIT")
			quit_command(server, reply_socket, line);
		else
			return ;
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	}
}

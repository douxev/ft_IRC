#include "ft_irc.hpp"
#include <cstddef>
#include <exception>
#include "numeric_replies.hpp"
#include <ctime>
#include <ostream>
#include <sstream>
#include <string>

void	init_client( Server& server, int reply_socket, std::string message) {

	(void) server;
	(void) message;
	std::stringstream msg_to_send;
	std::time_t result = std::time(NULL);
	
	//Parsing de message
	std::istringstream msg(message);
	std::string username;
	std::getline(msg, username, ' ');
	if (!server.nick_already_taken(username))
		server.get_user_class(reply_socket).set_name(username); //set username
	else
		std::cout << "NICKNAME ALREADY TAKEN" << std::endl;

	std::getline(msg, username, ' ');
	server.get_user_class(reply_socket).set_realname(username); //set realname


	msg_to_send.str("");
	msg_to_send << RPL_WELCOME << server.get_user_class(reply_socket).get_name() << " :Welcome to the GuiRaMa Internet Relay Chat Network\n";
	if (ft_send(reply_socket, msg_to_send.str()) == -1)
		std::cerr << "[Server] Send error to client " << server.get_user_class(reply_socket).get_name() << ": " <<  strerror(errno)  << std::endl;

	msg_to_send.str("");
	msg_to_send << RPL_YOURHOST << server.get_user_class(reply_socket).get_name() << " :Your host is irc.guirama.42, running on version Server version: 1.0 \n";
	if (ft_send(reply_socket, msg_to_send.str()) == -1)
		std::cerr << "[Server] Send error to client " << server.get_user_class(reply_socket).get_name() << ": " <<  strerror(errno)  << std::endl;

	msg_to_send.str("");
	msg_to_send << RPL_CREATED << server.get_user_class(reply_socket).get_name() << " :This server was created " << std::asctime(std::localtime(&result)) << "\n";
	if (ft_send(reply_socket, msg_to_send.str()) == -1)
		std::cerr << "[Server] Send error to client " << server.get_user_class(reply_socket).get_name() << ": " <<  strerror(errno)  << std::endl;

	msg_to_send.str("");
	msg_to_send << RPL_MYNFO << server.get_user_class(reply_socket).get_name() << " :irc.guirama.42 Server version: 1.0 \n"/*<available user modes><available channel modes> [<channel modes with a parameter>]*/;
	if (ft_send(reply_socket, msg_to_send.str()) == -1)
		std::cerr << "[Server] Send error to client " << server.get_user_class(reply_socket).get_name() << ": " <<  strerror(errno)  << std::endl;

	msg_to_send.str("");
	msg_to_send << RPL_ISUPPORT << server.get_user_class(reply_socket).get_name() << " :Des trucs... \n"/* tous les parametres qu'on utilisera pour ISUPPORT */;
	if (ft_send(reply_socket, msg_to_send.str()) == -1)
		std::cerr << "[Server] Send error to client " << server.get_user_class(reply_socket).get_name() << ": " <<  strerror(errno)  << std::endl;

	// ft_send(reply_socket, "NICK :" + server.get_user_class(reply_socket).get_name());

}

//ADD TRY CATCH EXCEPTIONS FOR SENDING GOOD REPLIES
void	parse_commands( Server& server, int reply_socket, std::istringstream& message ) {

	std::string cmd;
	std::string line_str;

	std::vector<std::string> rev_lines;
	std::string rline;

	// std::cout << "User is: " << server.get_user_class(reply_socket).get_name() << std::endl;

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
		line.str(&line.str()[cmd.size() + 1]);
		try {

			if (!std::isprint(line.str().at(line.str().size() - 1)))
				line.str(line.str().substr(0, line.str().size() - 1)); //remove weird char

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
			else if (cmd == "PRIVMSG")
				privmsg_command(server, reply_socket, line);
			else if (cmd == "MODE")
				mode_command(server, reply_socket, line);
			else if (cmd == "WHO")
				who_command(server, reply_socket, line);
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
				std::cout << "[WARN!] Unknown Command: " << cmd << std::endl;
		}
		catch (std::exception& e) {
			std::cout << e.what() << std::endl;
		}
	}
}

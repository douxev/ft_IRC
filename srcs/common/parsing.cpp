#include "error_numeric.hpp"
#include "ft_irc.hpp"
#include <cstddef>
#include <exception>
#include "numeric_replies.hpp"
#include <ctime>
#include <ostream>
#include <sstream>
#include <string>

void	init_client( Server& server, int reply_socket, std::string message) {
	try
	{
		if (server.get_user_class(reply_socket).user_authenticate()) {
			ft_send(reply_socket, "462 " + server.get_user_class(reply_socket).get_name() + " :You may not reregister\r\n");
			return ;
		}
	}
	catch(const std::exception& e) {}
	try
	{
		User& user = server.get_user_class(reply_socket);
		if (user.get_password() == server.get_pass())
			user.authenticate();
		else {
			ft_send(reply_socket, "464 " + user.get_name() + " :Password incorrect\n");
			return ;
		}

	}
	catch(const std::exception& e){}
	
	std::stringstream msg_to_send;
	std::time_t result = std::time(NULL);
	
	//Parsing de message
	std::istringstream msg(message);
	std::string username;
	std::getline(msg, username, ' ');
	std::getline(msg, username, ' ');
	server.get_user_class(reply_socket).set_realname(username); //set realname

	std::string address;
	std::getline(msg, address, ' ');
	server.get_user_class(reply_socket).set_ip(address);

	msg_to_send << RPL_WELCOME << server.get_user_class(reply_socket).get_name() << " :Welcome to the GuiRaMa Internet Relay Chat Network\n";
	msg_to_send << RPL_YOURHOST << server.get_user_class(reply_socket).get_name() << " :Your host is irc.guirama.42, running on version Server version: 1.0 \n";
	msg_to_send << RPL_CREATED << server.get_user_class(reply_socket).get_name() << " :This server was created " << std::asctime(std::localtime(&result)) << "\n";
	msg_to_send << RPL_MYNFO << server.get_user_class(reply_socket).get_name() << " :irc.guirama.42 Server version: 1.0 \n"/*<available user modes><available channel modes> [<channel modes with a parameter>]*/;
	msg_to_send << RPL_ISUPPORT << server.get_user_class(reply_socket).get_name() << " :Des trucs... \n"/* tous les parametres qu'on utilisera pour ISUPPORT */;
	if (ft_send(reply_socket, msg_to_send.str()) == -1)
		std::cerr << SERVER_INFO << "Send error to client " << server.get_user_class(reply_socket).get_name() << ": " <<  strerror(errno)  << std::endl;

}

//ADD TRY CATCH EXCEPTIONS FOR SENDING GOOD REPLIES
void	parse_commands( Server& server, int reply_socket, std::istringstream& message ) {

	for (std::string line_str; std::getline(message, line_str);) {

		if (line_str.size() < 1) { // the string looks empty
			server.buffer[reply_socket].erase(0, line_str.size() + 1);
			continue;
		}
		if (line_str.at(line_str.size() - 1) != '\r') { // there is not CR
			return ;
		}

		server.buffer[reply_socket] = server.buffer[reply_socket].substr(line_str.size() + 1);

		std::istringstream line(line_str);
		std::string cmd;
		std::getline(line, cmd, ' ');
		if (line.str().size() >= cmd.size() + 1)
			line.str(&line.str()[cmd.size() + 1]);

		try {

			if (!std::isprint(line.str().at(line.str().size() - 1)))
				line.str(line.str().substr(0, line.str().size() - 1)); //remove \r char

			if (cmd == "PING")
				pong(reply_socket, line.str());
			else if (cmd == "CAP")
				cap_command(server, reply_socket, line);
			else if (cmd == "PASS")
				pass_command(server, reply_socket, line);
			else if (cmd == "NICK")
				nick_command(server, reply_socket, line.str());
			else if (cmd == "USER")
				init_client(server, reply_socket, line.str());
			else {
				try
				{
					User& user = server.get_user_class(reply_socket);
					if (!user.user_authenticate() && user.get_password() != server.get_pass()) {
						ft_send(reply_socket, "464 " + user.get_name() + " :Password incorrect\r\n");
							continue;
					}
				}
				catch(const std::exception& e)
				{
					std::cerr << RED << "[WARN!]" << RESET << e.what() << '\n';
					ft_send(reply_socket, "Not Connected, connection is not made by a user\r\n");
					return;
				}
				
				if (cmd == "MOTD")
					motd_command(server, reply_socket);
				else if (cmd == "VERSION")
					version_command(reply_socket);
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
				else if (cmd == "WHOIS")
					whois_command(server, reply_socket, line);
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
				else if (cmd == "QUIT") {
					quit_command(server, reply_socket, line);
					return ;
				}
				else 
					std::cout << RED << "[WARN!]" << RESET << " Unknown Command: " << cmd << std::endl;
			}
		}
		catch (const NoSuchNickException& e) {
			ft_send(reply_socket, ERR_NOSUCHNICK + server.get_user_class(reply_socket).get_name() + " " + line.str() + " :No such nick\r\n");
		}
		catch (const NoSuchChannelException& e) {
			ft_send(reply_socket, ERR_NOSUCHCHANNEL + server.get_user_class(reply_socket).get_name() + " " + line.str() + " :No such channel " + line.str() + "\r\n");
		}
		catch (const NotOnChannelException& e) {
			ft_send(reply_socket, ERR_NOTONCHANNEL + server.get_user_class(reply_socket).get_name() + " " + line.str() + " :You aren't on that channel\r\n");
		}
		catch (const NeedMoreParamsException& e) {
			ft_send(reply_socket, ERR_NEEDMOREPARAMS + server.get_user_class(reply_socket).get_name() + cmd + " :Not enough parameters\r\n");
		}
		catch (const ChanOPrivsNeededException& e) {
			ft_send(reply_socket, (std::string) ERR_CHANOPRIVSNEEDED + line.str() + " :You're not channel operator\r\n");
		}
		catch (std::exception& e) {}
	}
}

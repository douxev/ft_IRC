#include "bot/ft_bot.hpp"
#include <sstream>

void	parse_commands(Bot& bot, std::istringstream& line) {

	//! get the channel string in this string same for user
	std::string channel;
	std::string user;
	std::getline(line, user, ' ');
	if (user.size() > 1)
		user = user.substr(1);
	std::getline(line, channel, ' ');
	std::getline(line, channel, ' ');
	
	//! starting parsing the message
	std::string msg;
	std::getline(line, msg, ':');
	std::getline(line, msg);

	std::istringstream cmd_is;
	cmd_is.str(msg);
	std::string cmd;
	std::getline(cmd_is, cmd, ' ');
	if (line.str().size() >= cmd.size() + 1)
		line.str(&line.str()[cmd.size() + 1]);

	//!CHECK IF BOT IS OP

	//! NOT SURE WE HAVE TIME FOR ANYTHING OTHER THAN FORBIDDEN ADD AND REMOVE
	
	//?process like a private command
	if (cmd == "ADD") //? Add to channel
		add_cmd(bot, line);
	else if (cmd == "REMOVE") //? Remove from channel
		remove_cmd(bot, line);
	else if (cmd == "FORBIDDEN") //? Add or remove forbidden words
		forbidden_cmd(bot, line);
	else if (cmd == "TIME") //? Get time
		time_cmd(bot, line);
	else if (cmd == "LOG") //? Start logging on said Channel
		log_cmd(bot, line);
	else if (cmd == "UNLOG") //? Stop logging
		unlog_cmd(bot, line);
	else { //? Process like a general message
		line.str(msg);
		for (std::string word; std::getline(line, word, ' ')) {
			if (bot.forbidden(channel, word)) {
				bot.kick_user(channel, user, word);
			}
			//!Log messages in a file if LOG is ON?
		}
	}
}
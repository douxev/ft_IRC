#include "bot/ft_bot.hpp"
#include <sstream>
#include <iostream>

void	parse_commands(Bot& bot) {

	while (bot.buffer.size() > 0) {
		std::istringstream line;

		line.str(bot.buffer.front());
		bot.buffer.pop_front();

		std::cout << BOTINFO << line.str() << std::endl;

		// while (bot.buffer.size() > 0 && (!line.str().size() || line.str().at(0) != ':'))  {
		// 	line.str(bot.buffer.front());
		// 	bot.buffer.pop_front();
		// }
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

		std::string cmd;
		std::istringstream cmd_is;
		cmd_is.str(msg);
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
		else { //? Process like a general message
			line.str(msg);
			std::cout << "HERE: " << line.str() << std::endl; 
			for (std::string word; std::getline(line, word, ' ');) {

				if (bot.forbidden(channel, word)) {
					bot.kick_user(channel, user, word);
				}
				//!Log messages in a file if LOG is ON?
			}
		}
	}
}

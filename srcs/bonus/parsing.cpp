#include "bot/ft_bot.hpp"
#include <sstream>
#include <iostream>

void	parse_commands(Bot& bot) {

	while (bot.buffer.size() > 0) {
		std::istringstream line;

		line.str(bot.buffer.front());
		bot.buffer.pop_front();

		// if (!line.str().size() || line.str().at(0) != ':')
		// 	continue ;

		std::cout << RECV << line.str() << std::endl;



		std::string cmd;
		std::string target;
		std::string user;
		std::string msg;
		std::istringstream message;

		std::getline(line, user, ' ');
		user = user.substr(1);
		std::getline(line, cmd, ' ');
		std::getline(line, target, ' ');
		std::getline(line, msg);
		if (msg.size())
			message.str(msg.substr(1));
		else
			message.str(msg);
		if (target == bot.get_nick()) { 		//? Commands that targets the bot directly
			if (cmd == "INVITE") {
				bot.join_channel(msg);
				continue;
			}
		}
		else if (cmd == "PRIVMSG") {
			if (!bot.check_op(target)) {
				bot.not_op(target);
				continue;
			}
			for (std::string word; std::getline(message, word, ' ');) {
				
				if (word != "ADD" || word != "REMOVE") {
					std::cout << BOTINFO << "[CHECKS] " << word << std::endl;
					if (bot.forbidden(target, word))
						bot.kick_user(target, user, word);
					continue ;
				}
				
				std::getline(message, word);
				std::istringstream word_list(word);

				if (word == "ADD") 		//? Add forbidden words
					add_cmd(bot, user, target, word_list);
				if (word == "REMOVE") 		//? Remove forbidden words
					remove_cmd(bot, user, target, word_list);
				break ;
			}
		} else if (cmd == "PART" || cmd == "QUIT" || cmd == "KICK")
		{
			if (bot.is_alone(target)) {
				
			}

		}
		
	}
}

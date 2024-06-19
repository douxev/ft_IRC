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

		if (!std::isprint(line.str().at(line.str().size() - 1)))
			line.str(line.str().substr(0, line.str().size() - 1)); //remove weird char

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
				return ;
			}


			std::string word;
			for (; std::getline(message, word, ' ');) {
				if (word == "ADD" || word == "DEL" || word == "LIST")
					break ;
				std::cout << BOTINFO << "[CHECKS] " << word << std::endl;
				if (bot.forbidden(target, word))
					bot.kick_user(target, user, word);
			}
			std::string words;
			std::getline(message, words);
			if (word == "ADD" || word == "DEL" || word == "LIST") {

				std::getline(message, words);
				std::istringstream word_list(words);

				if (!bot.is_op(target, user))
					return ;
				if (target.size() > 0 && target.at(0) != '#')
					return ;

				if (word == "ADD") 		//? Add forbidden words
					add_cmd(bot, target, word_list);
				else if (word == "DEL") 		//? Remove forbidden words
					remove_cmd(bot, target, word_list);
				else if (word == "LIST")
					list_cmd(bot, target);
			}
		} else if (cmd == "PART"  || cmd == "KICK")	{
			if (bot.is_alone(target)) {
				bot.send("PART " + target + "\r\n");
				bot.leave_channel(target);

			}
		} else if (cmd == "QUIT") {
			std::map<std::string, std::vector<std::string> >map = bot.get_words_map();
			for (std::map<std::string, std::vector<std::string> >::iterator it = map.begin(); it != map.end(); it++)
				if (bot.is_alone(it->first)) {
					bot.send("PART " + it->first + "\r\n");
					bot.leave_channel(it->first);
				}
		}
		
	}
}

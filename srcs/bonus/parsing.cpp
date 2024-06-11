#include "bot/ft_bot.hpp"

void	parse_commands(Bot& bot, std::istringstream& message) {

	for (std::string line_str; std::getline(message, line_str);) {
		std::istringstream line(line_str);
		std::string cmd;
		std::getline(line, cmd, ' ');
		if (line.str().size() >= cmd.size() + 1)
			line.str(&line.str()[cmd.size() + 1]);


		//!CHECK IF BOT IS OP
		
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
	}
}
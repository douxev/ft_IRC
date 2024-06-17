#include <cctype>
#include <sstream>
#include "bot/ft_bot.hpp"
#include <iostream>

void	add_cmd( Bot& bot, std::istringstream& message ) {
	std::string channel;


	if (std::getline(message, channel)) {
		
		if (!bot.check_op(channel))
			bot.not_op(channel);

		if (channel.size() > 0 && channel.at(0) != '#')
			channel = "#" + channel;
		for (std::string word; std::getline(message, word, ' ');) {
			bot.add_word(channel, word);
		}
	}
}

void	remove_cmd( Bot& bot, std::istringstream& message ) {
	std::string channel;

	if (std::getline(message, channel, ' ')) {
		
		if (!bot.check_op(channel)) {
			bot.not_op(channel);
		}
		
		if (channel.size() && channel.at(0) != '#')
			channel = "#" + channel;
		for (std::string word; std::getline(message, word, ' ');) {
			bot.remove_word(channel, word);
		}
	}
}

void	time_cmd( Bot& bot, std::istringstream& message ) {
	(void) bot;
	std::cout << message.str() << std::endl;

}


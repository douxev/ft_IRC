#include <cctype>
#include <sstream>
#include "bot/ft_bot.hpp"
#include <iostream>

void	add_cmd( Bot& bot, std::string user, std::string channel, std::istringstream& word_list ) {
	std::string word;

		
	if (!bot.check_op(channel)) {
		bot.not_op(channel);
		return ;
	}

	if (channel.size() > 0 && channel.at(0) != '#') {
		return ;
	}
	for (; std::getline(word_list, word, ' ');) {
		bot.add_word(channel, word);
	}
	std::getline(word_list, word);
	bot.add_word(channel, word);
}

void	remove_cmd( Bot& bot, std::string user, std::string channel, std::istringstream& word_list ) {
	std::string word;

		
	if (!bot.check_op(channel)) {
		bot.not_op(channel);
		return ;
	}

	if (channel.size() > 0 && channel.at(0) != '#') {
		return ;
	}
	for (; std::getline(word_list, word, ' ');) {
		bot.remove_word(channel, word);
	}
	std::getline(word_list, word);
	bot.remove_word(channel, word);
}

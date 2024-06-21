#include <cctype>
#include <cstddef>
#include <sstream>
#include "bot/ft_bot.hpp"
#include <iostream>

void	add_cmd( Bot& bot, std::string channel, std::istringstream& word_list ) {
	std::string word;

	std::cout << BOTINFO << "ADD" << std::endl;
	for (;std::getline(word_list, word, ' ');) {
		if (word.empty())
				return ;
		bot.add_word(channel, word);
	}
}

void	remove_cmd( Bot& bot, std::string channel, std::istringstream& word_list ) {
	std::string word;

	for (;std::getline(word_list, word, ' ');) {
		if (word.empty())
				return ;
		bot.remove_word(channel, word);
	}
}

void	list_cmd( Bot& bot, std::string channel ) {
	std::vector<std::string>& words = bot.list_word(channel);
	std::stringstream message;

	std::cout << BOTINFO << "Forbidden words list query" << std::endl;

	message << " :List of forbidden words: ";

	for (size_t i = 0; i < words.size(); i++) {
		message << "'" << words[i] << "' ";
	}
	bot.send("PRIVMSG " + channel + message.str() + "\r\n");
}

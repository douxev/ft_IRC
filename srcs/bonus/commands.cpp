#include <sstream>
#include "bot/ft_bot.hpp"
#include <iostream>

void	add_cmd( Bot& bot, std::istringstream& message ) {
	(void) bot;
	std::cout << message.str() << std::endl;
}

void	remove_cmd( Bot& bot, std::istringstream& message ) {
	(void) bot;
	std::cout << message.str() << std::endl;

}

void	forbidden_cmd( Bot& bot, std::istringstream& message ) {
	(void) bot;
	std::cout << message.str() << std::endl;

}

void	time_cmd( Bot& bot, std::istringstream& message ) {
	(void) bot;
	std::cout << message.str() << std::endl;

}


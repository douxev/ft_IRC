#include "bot/ft_bot.hpp"
#include <exception>
#include <iostream>

//! ./bot HOST PORT PASSWORD
int main( int ac, char **av) {

	(void) av;
	if (ac < 4) {
		return (0);
	}

	//!CHECK ALL PARAM ARE GOOD BEFORE

	//!maybe allow for user set nickname
	Bot bot(av[1], av[2], av[3]);

	while (true) {
		try {
			bot.receive();
			parse_commands(bot);
		}
		catch (const std::exception& e) {
			std::cout << WARN << e.what() << std::endl;
		}
	}
}

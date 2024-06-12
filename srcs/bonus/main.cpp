#include "bot/ft_bot.hpp"


//! ./bot HOST PORT PASSWORD
int main( int ac, char **av) {

	(void) av;
	if (ac < 4) {
		return (0);
	}

	//!CHECK ALL PARAM ARE GOOD BEFORE

	//!maybe allow for user set nickname
	Bot bot(av[1], av[2], av[3]);



}

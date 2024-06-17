#include "bot/ft_bot.hpp"
#include <exception>
#include <iostream>
# include <signal.h>
# include <bits/sigaction.h>
#include <cstdlib>

static Bot* get_server( Bot* bot ) {
	static Bot* saved_bot = NULL;

	if (saved_bot && !bot)
		return (saved_bot);
	else
		saved_bot = bot;
	return NULL;
}

static void	sig_handler( int sig ) {
	(void) sig;
	std::cout << "\n" << BOTINFO << " SIG RECEIVED" << std::endl;
	get_server(NULL)->~Bot();
	std::exit(0);
}

static void set_signals( void ) {
	struct sigaction	sa = {};

	sa.sa_handler = &sig_handler;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
}

//! ./bot HOST PORT PASSWORD
int main( int ac, char **av) {

	(void) av;
	if (ac < 4) {
		return (0);
	}

	//!CHECK ALL PARAM ARE GOOD BEFORE

	//!maybe allow for user set nickname
	Bot bot(av[1], av[2], av[3]);
	get_server(&bot);
	set_signals();
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

#include "bot/ft_bot.hpp"
#include <iostream>
#include <cstdlib>

void	check_args( char **av ) {

	if (av[0] != "127.0.0.1") {
		std::cout << "Wrong ip adress" << std::endl;
		return;
	if (!isdigit(strtol(av[1], NULL, 0)) || sizeof(av[1]) != 4) {
		std::cout << "port must be 4 numerical value" << std::endl;
		return;
	}
	if (!isprint(av[2])) {
		std::cout << "password is wrong format" << std::endl;
		return ;
	}
	(void) av;
	
}

void ft_send( int reply_socket, std::string msg ) {
	//sends said message to server
}

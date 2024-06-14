#include "bot/ft_bot.hpp"
#include <iostream>

void	check_args( char **av ) {
	(void) av;
	
}

int	ft_send(int socketfd, std::string msg) {

	size_t sent = 0;
	size_t send_value = 0;

	const size_t len = msg.size();

	if (len > 2 && msg.at(msg.size() - 1) != '\n') {
		msg += "\n";
		std::cout << BOTINFO << "\033[0;33mADDED LINE\033[0m" << std::endl;
	}


	std::cout << GREEN << "[SENT" << socketfd << "] " << RESET << msg;
	while (sent < len) {
		send_value = send(socketfd, msg.c_str() + sent, len, 0);
		if (send_value < 0)
			return send_value;
		else
			sent += send_value;
	}
	return (0);
}

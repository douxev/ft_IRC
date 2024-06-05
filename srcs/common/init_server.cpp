
#include <sstream>
#include <string>
#include <stdlib.h>
#include <ft_irc.hpp>
#include <fcntl.h>


void	check_args(int ac, char **av) {
	(void) av;
	if (ac != 3)
		exit(1);
}

short	get_port(int ac, char **av) {
	std::stringstream port;

	(void) ac;
	port << av[1];

	return (std::strtod(port.str().c_str(), NULL));
}


int		create_server_socket( short port ) {
	struct sockaddr_in sa;
	
	memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET; // IPv4
    sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // 127.0.0.1, localhost
    sa.sin_port = htons(port);

	int socket_fd = socket(sa.sin_family, SOCK_STREAM, 0);
	if (socket_fd == -1) {
		std::cerr << "[Server] Socket error: " << strerror(errno) << std::endl;
		return (-1);
	}
	const int enable = 1;
	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
		std::cerr << "[Server] Socket option error: " << strerror(errno) << std::endl;
		return (-1);
	}

	std::cout << "[Server] Created server socket fd: " << socket_fd << std::endl;
	fcntl(socket_fd, F_SETFL, O_NONBLOCK);
	if (bind(socket_fd, (const struct sockaddr *)(&sa), sizeof(sa)))
	{
		std::cerr << "[Server] Bind error: " <<  strerror(errno) << std::endl;
        exit(1);
	}
	std::cout << "[Server] Bound server socket to port: " << port << std::endl;

	return(socket_fd);
}


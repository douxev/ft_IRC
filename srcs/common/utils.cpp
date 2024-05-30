#include <cstddef>
#include <sys/socket.h>
#include <exceptions.hpp>

void	ft_send(int socketfd, std::string msg) {

	size_t sent = 0;
	size_t send_value = 0;
	const size_t len = msg.size();
	while (sent < len) {
		send_value = send(socketfd, msg.c_str() + sent, len, 0);
		if (send_value < 0)
			throw SendErrorException();
		else
			sent += send_value;
	}
}

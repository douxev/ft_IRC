#include <sys/socket.h>
#include <exceptions.hpp>

void	ft_send(int socketfd, const void *c_str, size_t len) {

	size_t sent = 0;
	size_t send_value = 0;

	while (sent < len) {
		send_value = send(socketfd, c_str + sent, len, 0);
		if (send_value < 0)
			throw SendErrorException();
		else
			sent += send_value;
	}
}

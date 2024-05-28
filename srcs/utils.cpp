#include <sys/socket.h>
#include <exceptions.hpp>

void	ft_send(int socketfd, const void *c_str, size_t len) {

	size_t sent = 0;

	while (sent < len) {
		
		if (send(socketfd, c_str + sent, len, 0) < 0) {
			throw SendErrorException();
		}
	}
}

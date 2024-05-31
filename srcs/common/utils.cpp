#include <cstddef>
#include <sys/socket.h>
#include <exceptions.hpp>

int	ft_send(int socketfd, std::string msg) {

	size_t sent = 0;
	size_t send_value = 0;
	const size_t len = msg.size();
	while (sent < len) {
		send_value = send(socketfd, msg.c_str() + sent, len, 0);
		if (send_value < 0)
			return send_value;
		else
			sent += send_value;
	}
	return (0);
}

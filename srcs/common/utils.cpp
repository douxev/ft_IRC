#include "ft_irc.hpp"
#include <cstddef>
#include <sys/socket.h>
#include <exceptions.hpp>
#include "numeric_replies.hpp"

int	ft_send(int socketfd, std::string msg) {

	size_t sent = 0;
	size_t send_value = 0;
	const size_t len = msg.size();

	if (msg.at(msg.size() - 1) != '\n')
		msg.append("\n");
	std::cout << "[SENT" << socketfd << "] " << msg;
	while (sent < len) {
		send_value = send(socketfd, msg.c_str() + sent, len, 0);
		if (send_value < 0)
			return send_value;
		else
			sent += send_value;
	}
	return (0);
}

void	no_topic_set(int reply_socket, std::string channel) {
	ft_send(reply_socket, RPL_NOTOPIC + channel + " :No topic is set for " + channel + "\n");
}
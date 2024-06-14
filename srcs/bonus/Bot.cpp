#include "bot/Bot.hpp"
#include <climits>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include "bot/ft_bot.hpp"
#include <algorithm>
#include <sstream>
#include <string.h>
#include <netinet/in.h> // For sockaddr_in
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>


Bot::Bot( void ) {}

Bot::Bot( const Bot& Other ) {
	buffer = Other.buffer;
}



Bot::Bot( std::string host, std::string port, std::string password ): 
_host(host), _port(port), _pass(password), _nick("Marvin"), _username("Marvin"), _realname("Marvin") {
	init_connection();
}

Bot::Bot( std::string name, std::string host, std::string port, std::string password ): 
_host(host), _port(port), _pass(password), _nick(name), _username(name), _realname(name) {}


Bot& Bot::operator=( const Bot& Other ) {
	buffer = Other.buffer;
	return (*this);
}

Bot::~Bot() {

}

int 	Bot::init_connection( void ) {
	struct sockaddr_in sa;
	char* addr = (char *)_host.c_str();
	memset(&sa, 0, sizeof(sa));
<<<<<<< HEAD
	if (_host == "localhost") {
		std::string address = "127.0.0.1";
		addr = (char *) address.c_str();
	}
=======
	if (_host == "localhost")
		addr = "172.0.0.1";
>>>>>>> refs/remotes/origin/main
	if (inet_pton(AF_INET, addr, &(sa.sin_addr)) <= 0) {
		std::cerr << "[Client] Addr error: invalid address" << std::endl;
		return (-1);

	}
    sa.sin_family = AF_INET; // IPv4
    sa.sin_port = htons(atoi(_port.c_str()));

	int socket_fd = socket(sa.sin_family, SOCK_STREAM, 0);
	if (socket_fd == -1) {
		std::cerr << "[Client] Socket error: " << strerror(errno) << std::endl;
		return (-1);
	}

	if (connect(socket_fd, (const struct sockaddr *)(&sa), sizeof(sa)))
	{
		std::cerr << "[Client] Connect error: " <<  strerror(errno) << std::endl;
        exit(1);
	}
	std::cout << "[Client] Connected to server socket via port: " << _port << std::endl;
	_fd = socket_fd;

	//send authentification msg
	std::stringstream msg_to_send;
	msg_to_send << "CAP LS\n";
	if (!_pass.empty())
		msg_to_send << "PASS " << _pass << "\n";
	msg_to_send << "NICK " << _nick << "\n";
	msg_to_send << "USER "<< _username << " 0 * :" << _realname << "\n";
<<<<<<< HEAD
	this->send(msg_to_send.str());
=======
	ft_send(socket_fd, msg_to_send.str());
>>>>>>> refs/remotes/origin/main
	return(socket_fd);
}

bool Bot::check_op( void ) {
	return this->is_op(this->_nick);
}

//Does a WHOIS
bool Bot::is_op( std::string nick ) {
	std::string line;
	std::string rpl_code;

	size_t before = this->buffer.size();
	this->send("WHOIS " + nick);
	while (this->buffer.size() == before)
		this->receive();

	std::istringstream line_is;
	line_is.str(this->buffer.back());
	
	this->buffer.pop_back();

	std::getline(line_is, rpl_code, ' ');
	if (rpl_code != "311")
		notice( "received " + line_is.str() + " not 311 RPL_WHOISUSER.");
	std::getline(line_is, line, ' ');
	if (!line.empty() && line[0] == '@')
		return true;
	return false;
}

//joins when invited
void Bot::join_channel( std::string channel ) {

	(void) this->_channels[channel];
	this->notice("Joined or was already on " + channel);
}

//removes the channel from map
void Bot::leave_channel( std::string channel ) {

	this->_channels.erase(this->_channels.find(channel));
	this->notice("Left " + channel);
}

//adds word to the map
void Bot::add_word( std::string channel, std::string word ) {
	if (this->_channels.find(channel) == this->_channels.end())
		notice("Not on that channel.");
	else {
		if (std::find(this->_channels[channel].begin(), this->_channels[channel].end(), this->_channels[channel]) == this->_channels[channel].end()) {
			this->_channels[channel].push_back(word);
			notice("Added word '" + word + "' to " + channel + "'s forbidden words.");
		}
		else
			notice("Word '" + word + "' already registered for " + channel + ".");
	}
}

//removes word from map
void Bot::remove_word( std::string channel, std::string word ) {
	if (this->_channels.find(channel) == this->_channels.end())
		notice("Not on that channel.");
	else {
		if (std::find(this->_channels[channel].begin(), this->_channels[channel].end(), channel) == this->_channels[channel].end()) {
			this->_channels[channel].erase(std::remove(this->_channels[channel].begin(), this->_channels[channel].end(), word));
			notice("Word '" + word + "' has been removed for " + channel + ".");
		}
		else
			notice("word '" + word + "' not found on " + channel + ".");
	}
}

//Reads and parses PRIVMSG
void Bot::process_msg( std::istringstream& message ) {
	std::cout << RECV << message.str() << std::endl;


}

//kicks a user
void Bot::kick_user( std::string channel, std::string user, std::string fword ) {
	this->send("KICK " + channel + " " + user + ":You used a forbidden word: " + fword + "\n");
	notice("Kicked " + user + " from " + channel + " because of the forbidden word '" + fword + "'.");
}

//says it's not op on channel
void Bot::not_op( void ) {
	const size_t len = this->_not_op_channel.size();
	for (size_t i = 0; i < len; i++) {
		notice("The bot is a normal user on " + this->_not_op_channel[i]);
	}
}

//sends a PRIVMSG
void Bot::send( std::string msg ) {
	ft_send(this->_fd, msg);
	std::cout << SENT << msg << std::endl;
}

void Bot::notice( std::string msg ) {
	std::cout << BOTINFO << msg << std::endl;
}

void Bot::receive( void ) {
	char buffer[BUFSIZ];

	recv(this->_fd, &buffer, BUFSIZ, 0);

	std::istringstream buf;
	buf.str(buffer);
	for (std::string line; std::getline(buf, line);) {
		this->buffer.push_back(line);
	}
}

bool Bot::forbidden( std::string channel, std::string word ) {
	const size_t len = this->_channels[channel].size();
	for (size_t i = 0; i < len; i++) {
		if (this->_channels[channel][i] == word)
			return true;
	}
	return false;
}

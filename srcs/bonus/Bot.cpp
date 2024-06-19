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
#include <variant>
#include "Bot.hpp"


Bot::Bot( void ) {}

Bot::Bot( const Bot& Other ) {
	buffer = Other.buffer;
}

std::string Bot::get_nick( void ) {
	return this->_nick;
}

std::map<std::string, std::vector<std::string> > Bot::get_words_map(void)
{
    return _channels;
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
	if (this->_host == "localhost") {
		std::string address = "127.0.0.1";
		addr = (char *) address.c_str();
	}
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
	this->send(msg_to_send.str());
	return(socket_fd);
}

bool Bot::check_op( std::string channel ) {
	return this->is_op( channel, this->_nick);
}

bool Bot::is_alone(std::string channel)
{
	std::string line;
	std::string rpl_code;
	std::istringstream line_is;

	this->send("NAMES " + channel + "\r\n");
	this->receive();
	while (this->buffer.size() > 0) {
		this->receive();

		line_is.clear();
		line_is.str(this->buffer.back());
		this->buffer.pop_back();
		
		std::getline(line_is, rpl_code, ' ');
		while (rpl_code != "353" && this->buffer.size()) {
			std::getline(line_is, rpl_code, ' ');
			if (rpl_code == "353")
				break ;
			line_is.clear();
			line_is.str(this->buffer.back());
			this->buffer.pop_back();
		}
		if (rpl_code == "353")
			break ;
	}
	std::string names;
	std::getline(line_is, names, ':');
	std::getline(line_is, names);
	std::stringstream names_is;
	names_is.str(names);
	int i = 0;
	for (std::string word; std::getline(names_is, word, ' ') && word.at(0) != '\r';i++)
		std::cout << "[" + word + "]"  << std::endl;
	std::getline(line_is, line, ' ');
	std::cout << line << std::endl;
	if (i == 1)
		return true;
	return false;
} 
//Does a WHOIS
bool Bot::is_op( std::string channel, std::string nick ) {
	std::string line;
	std::string rpl_code;
	std::istringstream line_is;

	this->send("WHOIS " + nick + "\r\n");
	this->receive();
	while (this->buffer.size() > 0) {
		this->receive();

		line_is.clear();
		line_is.str(this->buffer.back());
		this->buffer.pop_back();
		
		std::getline(line_is, rpl_code, ' ');
		while (rpl_code != "319" && this->buffer.size()) {
			std::getline(line_is, rpl_code, ' ');
			if (rpl_code == "319")
				break ;
			line_is.clear();
			line_is.str(this->buffer.back());
			this->buffer.pop_back();
		}
		if (rpl_code == "319")
			break ;
	}
		std::getline(line_is, line, ':');
		std::getline(line_is, line);
		if (line.size() && line.at(0) != '#' && line.at(0) != '@')
			line = "#" + line;
		size_t pos = line.find_first_of(channel);
		// std::cout << "Parsed: " << line.at(pos) << std::endl;
		if (pos >= 1 && line.at(pos - 1) == '@') {
			return true;
		}
	
	return false;
}

// joins when invited
void Bot::join_channel( std::string channel ) {

	(void) this->_channels[channel];
	if (channel.size() > 0 && channel.at(0) != '#')
		channel = "#" + channel;
	this->send("JOIN " + channel + "\n");
	this->notice("Joined " + channel);
}

//removes the channel from map
void Bot::leave_channel( std::string channel ) {

	this->_channels.erase(this->_channels.find(channel));
	this->notice("Left " + channel);
}

//adds word to the map
void Bot::add_word( std::string channel, std::string word ) {
	(void) word;
	if (this->_channels.find(channel) == this->_channels.end())
		notice("Not on that channel.");
	else {
		if (std::find((this->_channels[channel]).begin(), 
			(this->_channels[channel]).end(), 
			word) == this->_channels[channel].end()) {
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
		if (std::find(this->_channels[channel].begin(), this->_channels[channel].end(), word) != this->_channels[channel].end()) {
			this->_channels[channel].erase(std::remove(this->_channels[channel].begin(), this->_channels[channel].end(), word));
			notice("Word '" + word + "' has been removed for " + channel + ".");
		}
		else
			notice("word '" + word + "' not found on " + channel + ".");
	}
}

std::vector<std::string>& Bot::list_word( std::string channel ) {
	return (this->_channels[channel]);
}


//Reads and parses PRIVMSG
void Bot::process_msg( std::istringstream& message ) {
	std::cout << RECV << message.str() << std::endl;


}

//kicks a user
void Bot::kick_user( std::string channel, std::string user, std::string fword ) {
	this->send("KICK " + channel + " " + user + " :You used a forbidden word: " + fword + "\n");
	notice("Kicked " + user + " from " + channel + " because of the forbidden word '" + fword + "'.");
}

//says it's not op on channel
void Bot::not_op( void ) {
	const size_t len = this->_not_op_channel.size();
	for (size_t i = 0; i < len; i++) {
		notice("The bot is a normal user on " + this->_not_op_channel[i]);
	}
}

void Bot::not_op( std::string channel ) {
	if (channel.size() > 2 && channel.at(0) != '#')
		channel = "#" + channel;
	std::cout << BOTINFO << "NOT OP on " << channel << std::endl;
	this->send("PRIVMSG " + channel + " :Not OP on this channel.");
}
//sends a PRIVMSG
void Bot::send( std::string msg ) {
	ft_send(this->_fd, msg);
}

void Bot::notice( std::string msg ) {
	std::cout << BOTINFO << msg << std::endl;
}

void Bot::receive( void ) {
	char buffer[BUFSIZ] = {};
	
	recv(this->_fd, buffer, BUFSIZ, 0);

	std::istringstream buf;
	buf.str(buffer);
	std::string line; 
	while (std::getline(buf, line)) {
		if (!line.empty())
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

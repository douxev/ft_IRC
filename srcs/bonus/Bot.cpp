#include "bot/Bot.hpp"
#include <iostream>
#include "bot/ft_bot.hpp"
#include <algorithm>
#include <sstream>

Bot::Bot( void ) {}

Bot::Bot( const Bot& Other ) {

}

Bot::Bot( std::string host, std::string port, std::string password ): _host(host), _port(port), _pass(password) {}


Bot& Bot::operator=( const Bot& Other ) {
	return (*this);
}

Bot::~Bot() {

}


//Does a WHOIS
bool Bot::is_op( std::string nick ) {
	std::string line;
	std::istringstream received;
	std::string rpl_code;

	this->send("WHOIS " + nick);

	std::getline(received, rpl_code, ' ');
	if (rpl_code != "311") {
		notice( "received " + received.str() + " not 311 RPL_WHOISUSER.");
	}
	std::getline(received, line, ' ');
	while (std::getline(received, line, ' ') && !line.empty()) {
		if (line[0] == '@')
			return true;
		else
			return false;
	}
	//while not endofWHOIS
	//knows whether its op or not

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

}

//sends a PRIVMSG
void Bot::send( std::string msg ) {
	ft_send(this->_fd, msg);
	std::cout << SENT << msg << std::endl;
}

void Bot::notice( std::string msg ) {
	std::cout << BOTINFO << msg << std::endl;
}

std::string Bot::recv( void ) {

}
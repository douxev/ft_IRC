#include "bot/Bot.hpp"
#include <iostream>
#include "bot/ft_bot.hpp"
#include <algorithm>

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

	//sends a whois msg to the server
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
		notice("Word '" + word + "' already registered for " + channel + ".");
	}
}

//removes word from map
void Bot::remove_word( std::string channel, std::string word ) {

}

//Reads and parses PRIVMSG
void Bot::process_msg( std::istringstream& message ) {

}

//kicks a user
void Bot::kick_user( std::string user ) {

}

//says it's not op on channel
void Bot::not_op( void ) {

}

//sends a PRIVMSG
void Bot::send( std::string name ) {

}

void Bot::notice( std::string msg ) {
	std::cout << BOTINFO << msg << std::endl;
}

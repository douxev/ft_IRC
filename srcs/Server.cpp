#include "Server.hpp"

Server::Server( void ) {}

Server& Server::operator=( const Server& Other ) {}

Server::Server( const Server& copied ) {}

Server::~Server() {}

void Server::user_identify( std::string password ) {

}

void Server::user_register( const User& name, std::string password, std::string email ) {

}

bool Server::_is_register( const User& name ) {

}

void Server::_add_active_channel( const Channel& channel ) {

}

void Server::_remove_unactive_channel( const Channel& channel ) {

}

void Server::_welcome_message() {

}
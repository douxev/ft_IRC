#ifndef SERVER_HPP
# define SERVER_HPP

# include <vector>
# include <map>
# include <string>
# include "Channel.hpp"

class Server {

public:

	Server( void );
	Server& operator=( const Server& Other );
	Server( const Server& copied);
	~Server();
	void user_identify( std::string password );
	void new_user( std::string name, int socketfd, std::string join_message, std::string quit_message, int ip_adress );
	void user_quit( const User& name );

private:

	void _add_active_channel( const Channel& channel );
	void _remove_unactive_channel( const Channel& channel );
	void _welcome_message();

	std::string							_ip_str;
	int									_ip_address;
	short								_port;
	int 								_socket;
	std::vector<Channel>				_active_channel;
	std::vector<User>					_connected_users;

};

#endif

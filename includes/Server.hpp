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
	void user_identify(std::string password);
	void user_register();


private:

	bool _is_register( const User& name );
	void _add_active_channel( const Channel& channel );
	void _remove_unactive_channel( const Channel& channel );
	void _welcome_message();


	std::string				_ip_str;
	int						_ip_address;
	short					_port;
	std::vector<Channel>	_active_channel;
	std::map<std::string, std::string> _registered_users;

};



#endif

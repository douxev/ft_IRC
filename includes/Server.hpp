#ifndef SERVER_HPP
# define SERVER_HPP

# include <vector>
# include <string>
# include <map>
# include "Channel.hpp"

class Server {

public:

	Server( void );
	Server& operator=( const Server& Other );
	Server( const Server& copied);
	~Server();
	void new_user( std::string name, int socketfd, std::string join_message, std::string quit_message, int ip_adress );
	void user_quit( const User& name );
	int _init_server(int ac, char **av);
	void	_manage_loop();

private:

	void _add_active_channel( const Channel& channel );
	void _remove_unactive_channel( const Channel& channel );
	void _welcome_message();
	void	_accept_connection();
	void	_read_data(int i);

	std::string							_ip_str;
	int									_ip_address;
	short								_port;
	int 								_server_socket;
	std::vector<Channel>				_active_channel;
	std::map<int, std::string>			_connected_users;
	std::vector<struct pollfd> 			_sockets_fds;
	int									_nb_sockets;
};

#endif

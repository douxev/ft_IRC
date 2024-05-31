#ifndef SERVER_HPP
# define SERVER_HPP

# include <vector>
# include <algorithm>
# include <string>
# include <map>
# include "Channel.hpp"

class Server {

public:

	Server( void );
	Server& operator=( const Server& Other );
	Server( const Server& Other );
	~Server();

	std::string get_motd( void );
	void	new_user( std::string name, int socketfd, int ip_adress );
	void	user_quit( const User* name );
	int		init_server( int ac, char **av );
	void	manage_loop( void );
	bool	nick_already_taken( std::string name ) const;
	void	change_nick( std::string name );
	User*	find_user_from_fd( int socketfd ) const ;
	
	void	join_channel( std::string name, std::string channel );
	void	part_channel( std::string name, std::string channel, std::string part_message );

	User&		_get_user_class( std::string name );
	Channel&	_get_channel_class( std::string name );

private:

	void 		_add_active_channel( const Channel *channel );
	void 		_remove_unactive_channel( const Channel *channel );
	void		_accept_connection( void );
	void		_read_data( int i );

	std::string							_motd;
	std::string							_ip_str;
	int									_ip_address;
	short								_port;
	int 								_server_socket;
	std::vector<Channel *>				_active_channels;
	std::vector<User *>					_connected_users;
	std::vector<struct pollfd> 			_sockets_fds;
	int									_nb_sockets;
};

#endif

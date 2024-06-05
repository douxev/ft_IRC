#ifndef SERVER_HPP
# define SERVER_HPP

#include "User.hpp"
#include "Channel.hpp"
#include <algorithm>

class Server {

public:

	Server( void );
	Server& operator=( const Server& Other );
	Server( const Server& Other );
	~Server( void );

	std::string get_motd( void );
	int		init_server( int ac, char **av );
	void	manage_loop( void );
	bool	nick_already_taken( std::string name ) const;
	void	change_nick( User& user, std::string name );
	
	bool	is_on_channel( std::string channel, std::string user );
	bool	is_op( std::string channel, std::string user );
	bool	channel_exists( std::string channel );


	void	send_all( std::string msg );
	void	send_all( int reply_socket, std::string msg );
	void	join_channel( std::string name, std::string channel );
	void	part_channel( std::string name, std::string channel, std::string part_message );

	User&		get_user_class( int socketfd );
	User&		get_user_class( std::string name );
	Channel&	get_channel_class( std::string name );
	std::string get_topic( std::string channel );
	std::string get_ip( void );
	std::string get_pass( void );

	void		set_topic( std::string channel, std::string topic, std::string topic_whotime );
	void		send_channel( std::string channelname, int sender_fd, std::string msg );
	void		remove_poll_fd( int fd );

	std::vector<Channel*> get_channels_list();
	std::vector<User*> get_connected_user();
private:

	void 		_add_active_channel( Channel *channel );
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
	std::string 						_password;
};

#endif

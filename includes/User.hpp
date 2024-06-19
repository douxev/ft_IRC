#ifndef USER_HPP
# define USER_HPP

# include <string>
# include <vector>
// # include <algorithm>
# include "Channel.hpp"

class Channel;


class User {

public:

	const std::string 		get_name( void ) const;
	const std::string		get_realname( void ) const;
	void					set_name( std::string name );
	void					set_realname( std::string name );
	void					set_fd( int fd );
	void					set_ip( std::string ip );
	void					set_password( std::string pass );
	std::string				get_ip( void ) const;
	std::string				get_password();
	int						get_socketfd( void ) const;
	std::vector<Channel*>&	get_list_channel();
	void					add_channel_list(Channel& channel);
	void					remove_channel_list(Channel* channel);
	bool					user_authenticate();

	void					authenticate();
	User( void );
	User& operator=( const User& Other );
	bool operator==( const User& Other ) const;
	User( const User& Other );
	~User();

private:

	void					_change_op_nick( const std::string new_name );

	std::string				_ip_address;
	int						_socketfd;
	std::string				_name;
	std::string				_realname;
	std::vector<Channel*>	_joined_channel;
	bool					_user_authenticate;
	std::string				_sent_password;

};



#endif

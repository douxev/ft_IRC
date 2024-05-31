#ifndef USER_HPP
# define USER_HPP

# include <string>

class User {

public:

	const std::string get_name( void ) const;
	void		change_name( std::string name );
	void		set_fd( int fd );
	int			get_socketfd( void ) const;
	std::vector<Channel*> get_list_channel();
	void		add_channel_list(Channel* channel);
	void		remove_channel_list(Channel* channel);

	User( void );
	User& operator=( const User& Other );
	bool operator==( const User& Other ) const;
	User( const User& copied);
	~User();

private:

	int			_ip_address;
	int			_socketfd;
	std::string	_name;
	std::string _realname;
	std::vector<Channel*> _joined_channel;

};



#endif

#ifndef USER_HPP
# define USER_HPP

# include <string>

class User {

public:

	std::string get_name( void ) const;
	std::string	get_quit_message( void ) const;
	std::string get_join_message( void ) const;
	int			get_socketfd( void ) const;

	User( void );
	User& operator=( const User& Other );
	bool operator==( const User& Other );
	User( const User& copied);
	~User();

private:

	std::string	_join_message;
	std::string	_quit_message;
	int			_ip_address;
	int			_socketfd;
	std::string	_name;


};



#endif

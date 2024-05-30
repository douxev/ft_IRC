#ifndef USER_HPP
# define USER_HPP

# include <string>

class User {

public:

	std::string get_name( void ) const;
	void		change_name( std::string name );
	void		set_fd( int fd );
	int			get_socketfd( void ) const;

	User( void );
	User& operator=( const User& Other );
	bool operator==( const User& Other ) const;
	User( const User& copied);
	~User();

private:

	int			_ip_address;
	int			_socketfd;
	std::string	_name;


};



#endif

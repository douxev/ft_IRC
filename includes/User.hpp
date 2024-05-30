#ifndef USER_HPP
# define USER_HPP

# include <string>

class User {

public:

	std::string get_name( void ) const;
	std::string	get_quit_message( void ) const;
	std::string get_join_message( void ) const;
	void		set_quit_message( std::string msg );
	void		set_join_message( std::string msg );
	void		change_name( std::string name );
	
	int			get_socketfd( void ) const;

	User( void );
	User& operator=( const User& Other );
	bool operator==( const User& Other ) const;
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

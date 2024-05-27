#ifndef USER_HPP
# define USER_HPP

# include <string>

class User {

public:

	User( void );
	User& operator=( const User& Other );
	User( const User& copied);
	~User();

private:

	
	int			_ip_address;
	std::string	_name;

};



#endif

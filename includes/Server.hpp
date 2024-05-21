#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>

class Server {

public:

	Server( void );
	Server& operator=( const Server& Other );
	Server( const Server& copied);
	~Server();

private:

	std::string	_ip_str;
	int			_ip_address;
	short		_port;

};



#endif

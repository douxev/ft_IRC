#ifndef BOT_HPP
# define BOT_HPP

#include <sstream>
#include <string>
#include <map>
#include <vector>

class Bot {

public:

	Bot( std::string host, std::string port, std::string password );
	Bot( std::string name, std::string host, std::string port, std::string password );
	Bot& operator=( const Bot& Other );
	Bot( const Bot& copied);
    ~Bot();

    int	 init_connection();
    bool forbidden( std::string channel, std::string word );
	bool check_op( void );
	bool is_op( std::string nick );
	void join_channel( std::string channel );
	void leave_channel( std::string channel );
	void add_word( std::string channel, std::string word );
	void remove_word( std::string channel, std::string word );
	void process_msg( std::istringstream& message );
	void kick_user( std::string channel, std::string user, std::string word );
	void not_op( void );
	void send( std::string msg );
	std::string recv( void );

	void notice( std::string msg );

private:

	Bot( void );

	const std::string 	_host;
	const std::string 	_port;
	const std::string 	_pass;
	int					_fd;

	std::string			_nick;
	const std::string	_username;
	const std::string	_realname;

	std::map<std::string, std::vector<std::string>>	_channels;
	std::vector<std::string>						_not_op_channel;

};



#endif

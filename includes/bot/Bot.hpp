#ifndef BOT_HPP
# define BOT_HPP

#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <list>

class Bot {

public:

	Bot( std::string host, std::string port, std::string password );
	Bot( std::string name, std::string host, std::string port, std::string password );
	Bot& operator=( const Bot& Other );
	Bot( const Bot& Other );
    ~Bot();

    int	 init_connection();
    bool forbidden( std::string channel, std::string word );
	bool check_op( std::string channel );
	bool is_op( std::string channel, std::string nick );
	bool is_alone(std::string channel);
	void join_channel( std::string channel );
	void leave_channel( std::string channel );
	void add_word( std::string channel, std::string word );
	void remove_word( std::string channel, std::string word );
	void process_msg( std::istringstream& message );
	void kick_user( std::string channel, std::string user, std::string word );
	
	std::vector<std::string>& list_word( std::string channel );

	void not_op( void );
	void not_op( std::string channel );
	
	void send( std::string msg );
	void receive( void );
	
	std::map<std::string, std::vector<std::string> > get_words_map( void );
	std::string get_nick( void );
	void notice( std::string msg );

	std::list<std::string>	buffer;

private:

	Bot( void );

	std::string 		_host;
	const std::string 	_port;
	const std::string 	_pass;
	int					_fd;

	std::string			_nick;
	const std::string	_username;
	const std::string	_realname;

	std::map<std::string, std::vector<std::string> >	_channels;
	std::vector<std::string>							_not_op_channel;

};
#endif

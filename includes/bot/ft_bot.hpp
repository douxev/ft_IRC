#ifndef FT_BOT_HPP
# define FT_BOT_HPP


# include <string>
// # include <vector>
#include <sstream>
#include <sys/socket.h> // For socket functions
#include <sys/types.h>	// For socket read/write
#include <netinet/in.h> // For sockaddr_in
# include "Bot.hpp"


# define RESET "\033[0m"
# define YELLOW "\033[0;33m"
# define RED "\033[0;91m"
# define GREEN "\033[0;32m"
# define BOTINFO "\033[0;91m[BOT]\033[0m "
# define RECV "\033[0;33m[RECV]\033[0m "
# define SENT "\033[0;32m[SENT]\033[0m "
# define WARN "\033[0;95m[WARN]\033[0m "

void	check_args( char **av );

void	add_cmd( Bot& bot, std::string channel, std::istringstream& word_list );
void	remove_cmd( Bot& bot, std::string channel, std::istringstream& word_list );
void	list_cmd( Bot& bot, std::string channel );


int		ft_send( int reply_socket, std::string msg );
void	parse_commands(Bot& bot);


#endif

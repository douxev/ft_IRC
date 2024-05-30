/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdoukhan <jdoukhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 12:59:03 by jdoukhan          #+#    #+#             */
/*   Updated: 2024/05/30 18:58:25 by jdoukhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <vector>
# include <string>
# include "User.hpp"
# include "ft_irc.hpp"

typedef enum e_modes {
	INVITE,
	TOPIC,
	KEY,
	OP,
	LIMIT
}	t_enum_modes;

typedef struct s_modes {
	bool 		invite_only;
	bool 		op_topic;
	std::string	password;
	int			limit;
}	t_modes;

class Channel {

public:

	void send_message( const User& user, const std::string msg );
	void server_message( const std::string msg );

	void force_op( const User& user);
	void change_role( const User& user, const User& target, bool is_op );
	void user_join( const User& user );
	void user_quit( const User& user, const std::string quit_message );
	void user_kicked( const User& user, std::string kick_message );

	void set_mode( t_enum_modes mode, size_t value );
	void set_mode( t_enum_modes mode, const User& user, const std::string target, bool value );
	void set_mode( t_enum_modes mode, bool value, std::string password );

	std::string get_name( void );
	

	Channel( const std::string name, const User& user, std::string join_message );
	Channel& operator=( const Channel& Other );
	bool operator==( const Channel& Other );
	Channel( const Channel& copied );
	~Channel();

private:

	Channel( void );
	bool _is_op( const User& name );
	bool _is_op( const std::string user );
	void _remove_connected_user( const User& user );
	void _add_connected_user( const User& user );
	
	t_modes						_modes;
	std::string					_topic;
	std::string					_name;
	std::vector<std::string> 	_op_users;
	std::vector<User> 			_connected_users;

};

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbonnard <gbonnard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 12:59:03 by jdoukhan          #+#    #+#             */
/*   Updated: 2024/06/07 12:15:51 by gbonnard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <vector>
# include <string>
// # include "User.hpp"

class User;
class Server;

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

	void	send_channel( const std::string msg );
	void	send_channel( int sender_fd, const std::string msg );
	void	send_userlist( const User& user );
	void	send_who( Server& server, int reply_socket );
	void	force_op( const User& user);
	void	change_role( const User& user, const User& target, bool is_op );
	void	print_ops( void );
	void	change_op_nick( const std::string user, const std::string new_name );



	void	user_join( User& user );
    void	user_quit(const User &user, const std::string quit_message);
    void	user_part(const User &user, const std::string part_message);
    void	user_kicked( const User& user, const User& target, std::string kick_message );


	std::string	user_count( void );
	bool	is_on_channel( const std::string username );

	void	set_mode( t_enum_modes mode, size_t value );
	void	set_mode( t_enum_modes mode, const User& user, const std::string target, bool value );
	void	set_mode( t_enum_modes mode, bool value, std::string password );
	bool	topic_mode_is_off ();

	std::string get_name( void );
	std::string get_topic( void );
	std::string	get_modes( void );
	void set_topic( std::string topic, std::string topic_whotime );


	Channel( const std::string name, User& user );
	Channel& operator=( const Channel& Other );
	bool operator==( const Channel& Other );
	Channel( const Channel& copied );
	~Channel();
	bool is_op( const User& name );
	bool is_op( const std::string user );

private:

	Channel( void );
	void _remove_connected_user( const User& user );
	void _add_connected_user( User& user );
	
	t_modes						_modes;
	std::string					_topic;
	std::string					_topic_whotime;
	std::string					_name;
	std::vector<std::string> 	_op_users;

public:
	std::vector<User *> 		_connected_users;

};

#endif

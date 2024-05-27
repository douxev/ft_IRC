/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdoukhan <jdoukhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 12:59:03 by jdoukhan          #+#    #+#             */
/*   Updated: 2024/05/27 14:04:52 by jdoukhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <vector>
# include <string>
# include "User.hpp"

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

	void send_message( const User& user, const std::string msg ) ;
	void change_role( const User& user, const User& target, bool is_op );
	void user_quit( const User& user );
	void user_kicked( const User& user );
	void set_mode( t_enum_modes mode );

	Channel( void );
	Channel& operator=( const Channel& Other );
	Channel( const Channel& copied );
	~Channel();


private:

	void _is_op( const User& name );

	t_modes						_modes;
	std::string					_topic;
	std::vector<std::string> 	_op_users;
	std::vector<User *> 		_connected_users;

};



#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdoukhan <jdoukhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 14:56:20 by jdoukhan          #+#    #+#             */
/*   Updated: 2024/05/28 15:03:30 by jdoukhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_HPP
# define FT_IRC_HPP


# include "Server.hpp"
# include "User.hpp"
# include "Channel.hpp"

void	ft_send(int socketfd, const void *c_str, size_t len);



#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minilibft_int.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aauthier <aauthier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 11:19:53 by jdoukhan          #+#    #+#             */
/*   Updated: 2024/05/10 02:01:23 by aauthier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINILIBFT_INT_H
# define MINILIBFT_INT_H

# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdarg.h>

int					ft_printf(const char *str, ...);
void				ft_putstr(char *str, size_t *z);
void				ft_putchar(unsigned char c, size_t *z);
void				ft_ptr(void *ptr, size_t *z);
void				ft_nbaseu(size_t nbr, char *base, size_t *z);
void				ft_nbase(int nbr, char *base, size_t *z);

#endif
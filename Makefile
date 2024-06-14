# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jdoukhan <jdoukhan@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/07 16:11:15 by jdoukhan          #+#    #+#              #
#    Updated: 2024/06/14 15:33:07 by jdoukhan         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


################################################################################
#                                     CONFIG                                   #
################################################################################

NAME = irc_serv
B_NAME = bot
CFLAGS = -Wall -Wextra -Werror -std=c++98 -g3
CC := c++
URL = https://cdn.intra.42.fr/pdf/pdf/89250/en.subject.pdf

################################################################################
#                                 PROGRAM'S SRCS                               #
################################################################################

COMMON_SOURCES = \
		main.cpp\
		init_server.cpp\
		Channel.cpp\
		User.cpp\
		Server.cpp\
		utils.cpp\
		commands.cpp\
		parsing.cpp\
		

M_SOURCES = 

B_SOURCES = main.cpp\
			commands.cpp\
			parsing.cpp\
			Bot.cpp\
			utils.cpp

################################################################################
#                                  ExtLib Body                                 #
################################################################################

INCLUDES = -I./includes -I./includes/bot

################################################################################
#                                 Makefile Body                                #
################################################################################

PM_SOURCES = $(addprefix common/, $(COMMON_SOURCES)) \
	$(addprefix mandatory/, $(M_SOURCES))
PB_SOURCES = $(addprefix bonus/, $(B_SOURCES))
SRCS = $(addprefix srcs/, $(PM_SOURCES))
OBJS = $(SRCS:srcs/%.cpp=objs/%.o)
B_SRCS = $(addprefix srcs/, $(PB_SOURCES))
B_OBJS = $(B_SRCS:srcs/%.cpp=objs/%.o)

all: $(NAME)

bonus: $(B_NAME)

objs/%.o : srcs/%.cpp
	@mkdir -p objs/common/ objs/mandatory objs/bonus
	@$(CC) $(CFLAGS) -MMD -c -o $@ $< $(INCLUDES)

$(B_NAME) : $(B_OBJS)
	@$(CC) $(B_SRCS) $(CFLAGS) -o $(B_NAME) $(INCLUDES)
	@echo "\033[0;32mBonus part compiled ✔️\033[0m"

$(NAME) : $(OBJS)
	@$(CC) $(SRCS) $(CFLAGS) -o $(NAME) $(INCLUDES)
	@echo "\033[0;32mMandatory part compiled ✔️\033[0m"

clean:
	@rm -rf $(OBJS) $(B_OBJS)
	@echo "\033[0;33mCleaned .o files ✔️\033[0m"

fclean: clean
	@rm -rf $(NAME)
	@echo "\033[0;91mCleaned exe ✔️\033[1;37m"

re: fclean all

subject.txt: 
	@curl $(URL) | pdftotext - subject.txt -layout -nopgbrk

read: subject.txt
	@bat subject.txt

.PHONY: all clean fclean re bonus read

-include $(OBJS:.o=.d);

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/24 08:18:44 by kbaridon          #+#    #+#              #
#    Updated: 2025/06/24 11:54:51 by kbaridon         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

GREEN=\033[0;32m
ORANGE=\033[38;5;214m
CPP = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -I $(SRCDIR)/include
SRCDIR = srcs

NAME = webserv
HEADER = $(SRCDIR)/include/Webserv.hpp

SRC = main.cpp utils.cpp request.cpp

OBJ = $(addprefix $(SRCDIR)/, $(SRC:.cpp=.o))

all:		$(NAME)

$(NAME):	$(OBJ)
			@echo "$(ORANGE)Compiling webserv...";
			@$(CPP) $(CPPFLAGS) $(OBJ) -o $(NAME)
			@echo "$(GREEN)Compilation completed !"


%.o:	%.cpp
			@$(CPP) $(CPPFLAGS) -c $< -o $@

$(OBJ):		$(HEADER)

clean:
			@echo "$(ORANGE)Cleaning objects..."
			@rm -rf $(OBJ)
			@echo "$(GREEN)Cleaning of objects completed !"

fclean:		clean
			@echo "$(ORANGE)Cleaning executables..."
			@rm -rf $(NAME)
			@echo "$(GREEN)Cleaning of executables completed !"

re:			fclean $(NAME)

.PHONY: all clean fclean re
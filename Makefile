# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pmateo <pmateo@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/24 08:18:44 by kbaridon          #+#    #+#              #
#    Updated: 2025/06/26 01:19:56 by pmateo           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

GREEN=\033[0;32m
ORANGE=\033[38;5;214m
RESET=\033[0m
CPP = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -I $(SRCDIR)/include
SRCDIR = srcs

NAME = webserv
HEADER = $(SRCDIR)/include/Webserv.hpp $(SRCDIR)/include/Message.hpp

SRC = main.cpp utils.cpp wait_request.cpp Message.cpp

OBJ = $(addprefix $(SRCDIR)/, $(SRC:.cpp=.o))

all:		$(NAME)

$(NAME):	$(OBJ)
			@echo "$(ORANGE)Compiling webserv...$(RESET)";
			@$(CPP) $(CPPFLAGS) $(OBJ) -o $(NAME)
			@echo "$(GREEN)Compilation completed !$(RESET)"


%.o:	%.cpp
			@$(CPP) $(CPPFLAGS) -c $< -o $@

$(OBJ):		$(HEADER)

clean:
			@echo "$(ORANGE)Cleaning objects...$(RESET)"
			@rm -rf $(OBJ)
			@echo "$(GREEN)Cleaning of objects completed !$(RESET)"

fclean:		clean
			@echo "$(ORANGE)Cleaning executables...$(RESET)"
			@rm -rf $(NAME)
			@echo "$(GREEN)Cleaning of executables completed !$(RESET)"

re:			fclean $(NAME)

.PHONY: all clean fclean re
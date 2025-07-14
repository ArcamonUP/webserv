# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kbaridon <kbaridon@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/24 08:18:44 by kbaridon          #+#    #+#              #
#    Updated: 2025/07/09 16:17:51 by kbaridon         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

GREEN=\033[0;32m
ORANGE=\033[38;5;214m
RESET=\033[0m
CPP = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -g -I $(SRCDIR)/include
SRCDIR = srcs

NAME = webserv
HEADER = $(SRCDIR)/include/WebServ.hpp $(SRCDIR)/include/Config.hpp \
$(SRCDIR)/include/ServerConfig.hpp $(SRCDIR)/include/LocationConfig.hpp \
$(SRCDIR)/include/Message.hpp $(SRCDIR)/include/Request.hpp $(SRCDIR)/include/Response.hpp

SRC = main.cpp Utils.cpp ServerConfig.cpp Config.cpp LocationConfig.cpp Message.cpp Request.cpp \
		RequestUtils.cpp Response.cpp ResponseStatus.cpp ResponseUtils.cpp WaitRequest.cpp HandleMethod.cpp \
        InitServers.cpp HandleRequest.cpp AutoIndex.cpp UtilsGet.cpp Traductor.cpp Cgi.cpp

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

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eabdelha <eabdelha@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/09/15 08:46:38 by eabdelha          #+#    #+#              #
#    Updated: 2022/12/16 21:51:32 by eabdelha         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY : all clean fclean re 

CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98

NAME = webserv

SRC_PATH = ./src/
DEF_PATH = ./src/defs/
UTL_PATH = ./src/utils/

CLS_PATH = ./src/classes/
INC_PATH = ./src/includes/

OBJ_PATH = ./obj/

INCLUDES =	$(INC_PATH)ServerSettings.hpp	\
			$(INC_PATH)MiniStructs.hpp	\
			$(INC_PATH)exceptions.hpp	\
			$(INC_PATH)macros.hpp	\
			$(INC_PATH)utils.hpp	\
			\
			$(CLS_PATH)ConfigParser.hpp		\
			$(CLS_PATH)ServerLauncher.hpp		\
			$(CLS_PATH)RecvHandler.hpp		\
			$(CLS_PATH)SendHandler.hpp		\
			$(CLS_PATH)RequestParser.hpp		\
			$(CLS_PATH)RequestProcessor.hpp		\
			

SRC	=	main.cpp	\
		\
		ConfigParser.cpp	\
		ServerLauncher.cpp	\
		RecvHandler.cpp		\
		SendHandler.cpp		\
		RequestParser.cpp	\
		RequestProcessor.cpp \
		\
		utility.cpp			\
		build_response.cpp
	
OBJECTS = $(addprefix $(OBJ_PATH), $(SRC:.cpp=.o))

all: $(OBJ_PATH) $(NAME) $(STD_NAME)

$(NAME): $(OBJECTS) 
	@$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME)


$(OBJ_PATH)%.o: $(SRC_PATH)%.cpp $(INCLUDES) 
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ_PATH)%.o: $(DEF_PATH)%.cpp $(INCLUDES) 
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ_PATH)%.o: $(UTL_PATH)%.cpp $(INCLUDES) 
	$(CC) $(CFLAGS) -o $@ -c $<


$(OBJ_PATH) : 
	@mkdir $@

clean:
	@rm -rf  $(OBJ_PATH)

fclean: clean 
	@rm -f $(NAME) $(STD_NAME) 

re: fclean all

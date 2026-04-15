CPP = c++
CPPFLAGS = -g3 -Wall -Wextra -Werror -MMD -MP -std=c++98
NAME = btc

OBJ_DIR = ./.obj
SRC_DIR = ./sources
INC_DIR = ./includes

SRC =													\
        $(SRC_DIR)/main.cpp		\
        $(SRC_DIR)/BitcoinExchange.cpp		\

OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
DEP = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.d)

all: $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp Makefile
	mkdir -p $(OBJ_DIR)
	$(CPP) -I$(INC_DIR) $(CPPFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CPP) $(OBJ) -o $(NAME)

-include $(DEP)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

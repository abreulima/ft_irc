NAME        := ircserv
CXX         := c++
CXXFLAGS    := -Wall -Wextra -Werror -std=c++98 -MMD -MP -g3
INC         := -Iincs

SRCS        := $(addprefix srcs/, $(addsuffix .cpp, Main Server Client Parser Channel Handlers))
OBJ_DIR     := objs
OBJS        := $(SRCS:%.cpp=$(OBJ_DIR)/%.o)
DEPS        := $(OBJS:.o=.d)
MAKEFLAGS   += -s

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

go: all
	./$(NAME)

-include $(DEPS)

.PHONY: all clean fclean re g
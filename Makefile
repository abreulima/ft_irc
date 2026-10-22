NAME		:= ircserv

CXX			:= c++
CXXFLAGS	:= -Wall -Wextra -Werror -std=c++98 -MMD -MP -g3
INC			:= -Iincs
MAKEFLAGS	+= -s

SRCS_DIR	:= srcs
OBJS_DIR	:= objs
SRCS		:= $(addprefix $(SRCS_DIR)/, $(addsuffix .cpp, main Server Client Parser Channel Handlers replies))
OBJS		:= $(SRCS:$(SRCS_DIR)/%.cpp=$(OBJS_DIR)/%.o)
DEPS		:= $(OBJS:.o=.d)

-include $(DEPS)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

go: all
	./$(NAME) 6667 ""

.PHONY: all clean fclean re g
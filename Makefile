SOURCES		:= Main.cpp Server.cpp Client.cpp Command.cpp

all: $(SOURCES)
	c++ -g -std=c++98 -Wall -Werror -Wextra $(SOURCES) -o server
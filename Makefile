SOURCES		:= Main.cpp Server.cpp Client.cpp Parser.cpp

all: $(SOURCES)
	c++ -g -std=c++98 -Wall -Werror -Wextra $(SOURCES) -o server
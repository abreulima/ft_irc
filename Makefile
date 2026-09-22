
SOURCES		:= sources/main.cpp sources/Client.cpp sources/Server.cpp sources/Channel.cpp sources/Command.cpp

all: $(SOURCES)
	c++ $(SOURCES) -Wall -Werror -Wextra -o server
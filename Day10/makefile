CC = g++

CPPFLAGS = -pthread -fconcepts -I../utils -I../range-v3/include -std=gnu++20 -Wall -Wextra -Werror -ltbb
CPPFLAGS_OPT = -pthread -fconcepts -I../utils -I../range-v3/include -std=gnu++20 -O3 -Wall -Wextra -Werror -ltbb

all: 
	$(CC) -g part12.cpp -o part12 $(CPPFLAGS) 

fast: 
	$(CC)    part12.cpp -o part12 $(CPPFLAGS_OPT)
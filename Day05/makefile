CC = g++

CPPFLAGS = -pthread -I../utils -fconcepts -std=gnu++20 -Wall -Werror
CPPFLAGS_OPT = -pthread -I../utils -fconcepts -std=gnu++20 -O3 -Wall -Werror

all: 
	$(CC) -g part12.cpp -o part12 $(CPPFLAGS) 

fast: 
	$(CC)    part12.cpp -o part12 $(CPPFLAGS_OPT)
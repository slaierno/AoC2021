CC = g++

CPPFLAGS = -pthread -I../utils -fconcepts -std=gnu++20 -Wall -Werror
CPPFLAGS_OPT = -pthread -I../utils -fconcepts -std=gnu++20 -O3 -Wall -Werror

p1: 
	$(CC) -g part1.cpp -o part1 $(CPPFLAGS)

p2: 
	$(CC) -g part2.cpp -o part2 $(CPPFLAGS) 

p1o: 
	$(CC)    part1.cpp -o part1 $(CPPFLAGS_OPT)

p2o: 
	$(CC)    part2.cpp -o part2 $(CPPFLAGS_OPT) 

all: p1 p2

fast: p1o p2o
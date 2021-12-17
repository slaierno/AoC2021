CC = g++
# CC = /opt/gcc-latest/bin/g++

CPPFLAGS     = -pthread -fconcepts -I../range-v3/include -I../utils -std=gnu++20 -Wall -Werror -ltbb
CPPFLAGS_OPT = -pthread -fconcepts -I../range-v3/include -I../utils -std=gnu++20 -Wall -Werror -ltbb -O3

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
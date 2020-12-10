# 
CC = gcc
LIBS = 
DEPS = helpers.h card.h poker.h
OBJ = main.o helpers.o card.o poker.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $<
#	$(CC) -std=c99 -c -o $@ $<

StudPokerSim: $(OBJ)
	$(CC) -o $@ $^ $(LIBS)

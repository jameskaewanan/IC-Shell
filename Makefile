CC = gcc
CFLAGS = -O -Wall


all: icsh.c
	$(CC) $(CFLAGS) -o icsh icsh.c

clean:
	$(RM) icsh

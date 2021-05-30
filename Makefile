CC = gcc
CFLAGS = -O -Wall


all: icsh

icsh: icsh.c advisor.c
	$(CC) $(CFLAGS) -o icsh icsh.c advisor.c
	
clean:
	$(RM) icsh
	
clean:
	$(RM) advisor

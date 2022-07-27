# Makefile for folli.c 
SRC	= folli.c

INCLUDESRC	= folli.h 

OBJ	= folli.o

CC = cc
HOMEDIR = .
BINDIR = .
SRCDIR = $(HOMEDIR)/Folli.docker
MANDIR = .
#LIBDIR = 

# turn the last 3 lines on for debug compilation
LIBS =  -lm  
CFLAGS = -v  -g  -static

# turn the last 3 lines on for -O2 compilation (and turn above 3 lines off )
#LIBS =  -lm 
#CFLAGS = -v 


folli:	$(OBJ) 
	gcc  -o folli  $(OBJ)  $(LIBS) 

clean:
	rm *.o 

folli.o :      folli.h 

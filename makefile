C=gcc
CFLAGS=-I. -ansi -pedantic -Wall -std=c99 -D_XOPEN_SOURCE
ALL_OBJS= parse.o memmy.o filecoms.o dircoms.o repl.o


.PHONY : compile clean run tar
all: fatcat
compile : fatcat


fatcat : main.o $(ALL_OBJS)
	$(CC) $(CFLAGS) main.o $(ALL_OBJS) -o fatcat
	rm -f *.o
	
memmy.o : UTILS/memmy.*
	$(CC) $(CFLAGS) -o memmy.o -c ./UTILS/memmy.c
parse.o: REPL/parse.*
	$(CC) $(CFLAGS) -o parse.o -c ./REPL/parse.c
dircoms.o : UTILS/dircoms.*
	$(CC) $(CFLAGS) -o dircoms.o -c ./UTILS/dircoms.c
filecoms.o : UTILS/filecoms.*
	$(CC) $(CFLAGS) -o filecoms.o -c ./UTILS/filecoms.c
repl.o: REPL/repl.*
	$(CC) $(CFLAGS) -o repl.o -c ./REPL/repl.c	
main.o : main.c
	$(CC) $(CFLAGS) -o main.o -c main.c

clean :
	rm -f *.o *.x

tar :
	tar -cvf cloysta.tar *

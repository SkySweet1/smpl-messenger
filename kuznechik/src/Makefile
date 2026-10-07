CC = gcc
CFLAGS = -Wall -lm

all: clie serv

clie: clie.c kuznechik.c network.c
	$(CC) -o clie clie.c kuznechik.c network.c $(CFLAGS)

serv: serv.c network.c
	$(CC) -o serv serv.c network.c $(CFLAGS)

clean:
	rm -f clie serv

.PHONY: all clean
CC = gcc
CFLAGS = -Wall
LDFLAGS = -lX11
SHELL = /bin/zsh

objects = main.o

all: $(objects) 
	$(CC) $(CFLAGS) $^ -o bin/main $(LDFLAGS)

$(objects): %.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

run:
	bin/main

clean:
	rm -f *.o bin/main


CC = gcc
CFLAGS = -Wall
LDFLAGS = -lX11
SHELL = /bin/zsh

objects = main.o x_display.o

all: $(objects) 
	$(CC) $(CFLAGS) $^ --debug -o bin/main $(LDFLAGS)

$(objects): %.o: %.c 
	$(CC) $(CFLAGS) --debug -c $^ -o $@

run:
	bin/main

clean:
	rm -f *.o bin/main


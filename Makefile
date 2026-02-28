CC = gcc
CFLAGS = -Wall
LDFLAGS = -lX11
SHELL = /bin/zsh

sources = main.c x_display.c keyboard.c

objects = $(patsubst %.c,%.o,$(sources))

depends = $(patsubst %.c,%.d,$(sources))

.PHONY: all clean

all: $(objects)
	$(CC) $(CFLAGS) $^ --debug -o bin/main $(LDFLAGS)

-include $(depends)

%.o: %.c Makefile
	$(CC) $(CFLAGS) -MMD -MP --debug -c $< -o $@

# 
# all: $(objects) 
# 
# $(objects): %.o: %.c 
# 	$(CC) $(CFLAGS) --debug -c $^ -o $@
# 
# run:
# 	bin/main
# 
# clean:
# 	rm -f *.o bin/main


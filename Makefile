CC = gcc
CFLAGS = -Wall
LDFLAGS = -lX11 -lXext -lm
SHELL = /bin/zsh

sources = main.c x_display.c keyboard.c game.c double_vec2.c $\
		  doom_render.c sprite_system.c hash_str_int.c map_loader.c

objects = $(patsubst %.c,bin/%.o,$(sources))

depends = $(patsubst %.c,%.d,$(sources))

.PHONY: all clean

all: $(objects)
	$(CC) $(CFLAGS) $^ --debug -o bin/main $(LDFLAGS)

-include $(depends)

bin/%.o: %.c Makefile
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


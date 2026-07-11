CC = gcc
CFLAGS = -std=gnu17 -lpthread -lncurses -lm -ldl -Iincludes

ifeq ($(shell pkg-config --exists ncurses && echo yes),)
$(error ncurses not found. Please install it with your favorite package manager))
endif

bouzenadchip8: src/*.c | builds 
	$(CC) $(CFLAGS) -o builds/bouzenadchip8 src/*.c 
builds:
	mkdir -p builds

clean:
	rm -f bouzenadchip8

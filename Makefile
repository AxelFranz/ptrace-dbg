EXEC = fdbg dummy
CFLAGS = -Wall -Wextra -g -std=c11

all: $(EXEC)

dummy: dummy.c
	cc -g dummy.c -o dummy

clean: 
	rm -f $(EXEC) tmp

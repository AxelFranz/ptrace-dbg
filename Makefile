EXEC = fdbg
CFLAGS = -Wall -Wextra -Og -std=c11

all: $(EXEC)

clean: 
	rm $(EXEC)

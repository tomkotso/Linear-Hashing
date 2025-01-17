# compile with gcc
CC=gcc

# Compiler options:
CFLAGS = -g -Wall -Wextra -Werror
TEST = -f testing.csv -b 2

# set the name of the executable file to compile here
PROGRAM = mvote

OBJS = hash.o main.o voter.o list.o

$(PROGRAM): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(PROGRAM)


clean:
	rm -f $(PROGRAM) $(OBJS) $(LAB) $(LAB_OBJS)

run:
	./$(PROGRAM) $(TEST)

val:
	valgrind ./$(PROGRAM) $(TEST)
CC = g++
CFLAGS = -Wall -std=c++1z
DEPS = commandmode.h
OBJ = test.o
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

test: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^


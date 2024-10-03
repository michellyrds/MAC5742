TARGET=jumping_frogs

CFLAGS=-std=c++2a -g -Wall -pthread
CC=g++

# SIZE = M = N (number of frogs on each side)
SIZE=4

.PHONY: all
all: $(TARGET) run

$(TARGET): $(TARGET).cpp
	$(CC) -o $(TARGET) $(CFLAGS) $(TARGET).cpp

run:
	./$(TARGET) $(SIZE)
.PHONY: clean
clean:
	rm $(TARGET)

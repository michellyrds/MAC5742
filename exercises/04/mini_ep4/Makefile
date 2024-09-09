TARGET = long-problem

all: $(TARGET) $(TARGET)-fixed

$(TARGET): $(TARGET).c
	gcc -o $@ $^ -lm

$(TARGET)-solved: $(TARGET)-fixed.c
	gcc -o $@ $^ -lm

clean:
	rm -f $(TARGET) $(TARGET)-fixed

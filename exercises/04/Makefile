TARGET = leapfrog

all: $(TARGET) $(TARGET)-fixed

$(TARGET): $(TARGET).cpp
	g++ -o $@ $^ -lm

$(TARGET)-solved: $(TARGET)-fixed.cpp
	g++ -o $@ $^ -lm

clean:
	rm -f $(TARGET) $(TARGET)-fixed

TARGET = heat
CC = gcc
CFLAGS = -Wall -Wextra -O2

NVCC = /usr/bin/nvcc
CUDAPATH = /usr/local/cuda

NVCCFLAGS = -I$(CUDAPATH)/include
LFLAGS = -L$(CUDAPATH)/lib64 -lcuda -lcudart -lm
all: $(TARGET) cuda_$(TARGET)

$(TARGET): $(TARGET).o
	$(CC) $(CFLAGS) -o $(TARGET).o $(TARGET).c

$(TARGET).o: $(TARGET).c
	$(CC) $(CFLAGS) -c $(TARGET).c

cuda_$(TARGET):
	$(NVCC) $(NVCCFLAGS) $(LFLAGS) -o cuda_$(TARGET).o $(TARGET).cu

clean:
	rm -f $(TARGET) $(TARGET).o cuda_$(TARGET).o
	rm -f room.txt cuda_room.txt

environment:
	pyenv virtualenv 3.10.5 cuda
	pyenv local cuda
	pip install -r requirements.txt
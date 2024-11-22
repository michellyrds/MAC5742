TARGET = heat
CC = gcc
CFLAGS = -Wall -Wextra -O2 -ffloat-store -fno-fast-math

NVCC = /usr/bin/nvcc
CUDAPATH = /usr/local/cuda

NVCCFLAGS = -I$(CUDAPATH)/include # --gpu-architecture=compute_75 --gpu-code=sm_75 -ftz=false -prec-div=true -prec-sqrt=true --fmad=false -O0

LFLAGS = -L$(CUDAPATH)/lib64 -lcuda -lcudart -lm
all: $(TARGET) cuda_$(TARGET) cuda_$(TARGET)_37

$(TARGET): $(TARGET).o
	$(CC) $(CFLAGS) -o $(TARGET).o $(TARGET).c

$(TARGET).o: $(TARGET).c
	$(CC) $(CFLAGS) -c $(TARGET).c

cuda_$(TARGET):
	$(NVCC) $(NVCCFLAGS) $(LFLAGS) -o cuda_$(TARGET).o $(TARGET).cu

cuda_$(TARGET)_37:
	$(NVCC) $(NVCCFLAGS) $(LFLAGS) -o cuda_$(TARGET)_37.o $(TARGET)_37.cu

clean:
	rm -f $(TARGET) $(TARGET).o cuda_$(TARGET).o cuda_$(TARGET)_37.o

environment:
	pyenv virtualenv 3.10.5 cuda
	pyenv local cuda
	pip install -r requirements.txt
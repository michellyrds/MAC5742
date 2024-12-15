TARGET = sequential_julia
PARALLEL_TARGET = 1D_parallel_julia

CC = gcc
CFLAGS = -Wall -Wextra -O2
LFLAGS = -lm

N = 1000
N_PROCESSES = 5

all: $(TARGET) $(PARALLEL_TARGET)

$(TARGET): $(TARGET).o
	$(CC) $(CFLAGS) -o $(TARGET).o $(TARGET).c $(LFLAGS)

$(TARGET).o: $(TARGET).c
	$(CC) $(CFLAGS) -c $(TARGET).c $(LFLAGS)

$(PARALLEL_TARGET):
	smpicc -O3 $(PARALLEL_TARGET).c -o $(PARALLEL_TARGET)

mpi:
	mpicc -o $(PARALLEL_TARGET) $(PARALLEL_TARGET).c $(LFLAGS)

run:
	./$(TARGET).o $(N)

mpi-run: 
	mpirun -np $(N_PROCESSES) ./$(PARALLEL_TARGET) $(N)

smpi-run:
	smpirun -np $(N_PROCESSES) -hostfile ./simple_cluster_hostfile.txt -platform ./simple_cluster.xml ./$(PARALLEL_TARGET) $(N)

clean:
	rm -f $(TARGET) $(TARGET).o
	rm -f $(PARALLEL_TARGET) $(PARALLEL_TARGET).o
	rm -f julia.bmp
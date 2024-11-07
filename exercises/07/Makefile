NVCC = /usr/bin/nvcc
CUDAPATH = /usr/local/cuda

NVCCFLAGS = -I$(CUDAPATH)/include
LFLAGS = -L$(CUDAPATH)/lib64 -lcuda -lcudart -lm

VectorAdd:
	$(NVCC) $(NVCCFLAGS) $(LFLAGS) -o VectorAdd VectorAdd.cu

CheckVersion:
	$(NVCC) $(NVCCFLAGS) $(LFLAGS) -o CheckVersion CheckVersion.cu
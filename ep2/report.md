# EP2 – Resolvendo a Equação de Calor em Estado Estacionário Usando CUDA

Para este exercício-programa, será utilizado minha máquina local, um notebook com uma GPU NVIDIA GeForce GTX 1650 de 4 GB de memória.

As configurações utilizadas são:

System Details Report
---

#### Report details
- **Date generated:**                          	2024-11-04 21:17:06

#### Hardware Information:
- **Hardware Model:**                          	Acer Nitro AN515-44
- **Memory:**                                  	16,0 GiB
- **Processor:**                               	AMD Ryzen™ 7 4800H with Radeon™ Graphics × 16
- **Graphics:**                                	AMD Radeon™ Graphics
- **Graphics 1:**                              	NVIDIA GeForce GTX 1650
- **Disk Capacity:**                           	1,3 TB

#### Software Information:
- **Firmware Version:**                        	V1.04
- **OS Name:**                                 	Ubuntu 24.04.1 LTS
- **OS Build:**                                	(null)
- **OS Type:**                                 	64-bit
- **GNOME Version:**                           	46
- **Windowing System:**                        	X11
- **Kernel Version:**                          	Linux 6.8.0-48-generic

Versão do NVCC:
```console
michelly@ubuntu:~/.../exercises/07$ nvcc --version
nvcc: NVIDIA (R) Cuda compiler driver
Copyright (c) 2005-2023 NVIDIA Corporation
Built on Fri_Jan__6_16:45:21_PST_2023
Cuda compilation tools, release 12.0, V12.0.140
Build cuda_12.0.r12.0/compiler.32267302_0
```

Para rodar o script python, um *virtualenv* com a versão 3.10.5 do python foi utilizada. As bibliotecas instaladas no ambiente está no arquivo `requirements.txt`.

Para fins de reproduciubilidade, os testes serão feitos com 100 pontos e 1024 iterações.

./heat 100 1024
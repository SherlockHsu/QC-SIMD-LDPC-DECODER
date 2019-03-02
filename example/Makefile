SRCS = $(wildcard ./src/*.c)

OBJS = $(SRCS:.c = .o)

CC = icc

INC = -I./inc -I../inc

LIBS = -lm -lpthread -lmkl_rt -fopenmp ./lib/libsimd_ldpc.a ../lib/libqc_simd_ldpc_decoder.a

CCFLAGS = -Wall -O3 -march=core-avx512 -std=c99

OUTPUT = main

all:$(OUTPUT)

$(OUTPUT) : $(OBJS)
	$(CC) $^ -o $@ $(INC) $(LIBS)

%.o : %.c
	$(CC) -c $< $(CCFLAGS)

clean:
	rm -rf main *.o *.txt    #清除中间文件及生成文件

.PHONY:clean

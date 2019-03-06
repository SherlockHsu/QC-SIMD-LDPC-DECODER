CC = gcc# Compiler

SRCS = ./src/main.c

OBJS = $(SRCS:.c = .o)# Object Files

INCS = -I../../inc -I./inc# Head File Directory

LIBS = -lm ../../lib/libqc_simd_ldpc_decoder_x86.a ./lib/libnr5g_simd_ldpc_encoder_x86.a# Library Files

CCFLAGS = -Wall -O3 -std=c99 #-m64# Flags for Compiler

TARGET = main# Target Name

all:$(TARGET)

$(TARGET) : $(OBJS)
	$(CC) $^ -o $@ $(INCS) $(LIBS) $(CCFLAGS)

%.o : %.c
	$(CC) -c $< $(CCFLAGS)

clean:
	rm -rf main *.o *.txt

.PHONY:clean

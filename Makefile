# Compiler
CC = g++

# Compiler flags
CFLAGS = -Wall -g

# Build target executable
TARGET = tp2

all: $(TARGET)

$(TARGET): tp2.o approx_algs.o
	$(CC) $(CFLAGS) -o $(TARGET) tp2.o approx_algs.o

tp2.o: tp2.cpp approx_algs.hpp
	$(CC) $(CFLAGS) -c tp2.cpp

approx_algs.o: approx_algs.cpp approx_algs.hpp
	$(CC) $(CFLAGS) -c approx_algs.cpp

clean:
	$(RM) $(TARGET) *.o *~
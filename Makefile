# Compiler
CC = g++

# Compiler flags
CFLAGS = -Wall -g

# Build target executable
TARGET = tp2

all: $(TARGET)

$(TARGET): tp2.o approx_algs.o tsp_utils.o
	$(CC) $(CFLAGS) -o $(TARGET) tp2.o approx_algs.o tsp_utils.o

tp2.o: tp2.cpp approx_algs.hpp tsp_utils.hpp
	$(CC) $(CFLAGS) -c tp2.cpp

approx_algs.o: approx_algs.cpp approx_algs.hpp
	$(CC) $(CFLAGS) -c approx_algs.cpp

tsp_utils.o: tsp_utils.cpp tsp_utils.hpp
	$(CC) $(CFLAGS) -c tsp_utils.cpp

clean:
	$(RM) $(TARGET) *.o *~

CC := g++
CFLAGS := -Wall -g
TARGET := bench

# $(wildcard *.cpp /xxx/xxx/*.cpp): get all .cpp files from the current directory and dir "/xxx/xxx/"
CPPFILES := $(wildcard *.cc)
SFILES := $(wildcard  *.S)
OBJS := $(CPPFILES:.cc=.o) $(SFILES:.S=.o)


CPPFILES := main.cc benchmark.cc
# CCASMFILES := singel_stride.cc

# OBJS := perf.o main.o benchmark.o single_stride.o





$(TARGET): $(OBJS)
	$(CC) -o $@ $^

%.o : %.S
		$(CC) -o $@ $< -c -g

%.o : %.cc
		$(CC) -o $@ $< -c -g


# single_stride.o: single_stride.cc single_stride.S
# 	$(CC) -o $@ $< -c -g


all: $(TARGET)

clean:
	rm -rf $(TARGET) *.o

.PHONY: all clean


run: $(TARGET)
	./$(TARGET)
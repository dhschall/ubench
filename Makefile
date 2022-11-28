
CC := g++
CFLAGS := -Wall -g
TARGET := bench

# $(wildcard *.cpp /xxx/xxx/*.cpp): get all .cpp files from the current directory and dir "/xxx/xxx/"
SRCS := $(wildcard *.cc)
# $(patsubst %.cpp,%.o,$(SRCS)): substitute all ".cpp" file name strings to ".o" file name strings
OBJS := $(patsubst %.cc,%.o,$(SRCS))


$(TARGET): $(OBJS)
	$(CC) -o $@ $^
%.o: %.cpp
	$(CC) $(CFLAGS) -c $<


all: $(TARGET)

clean:
	rm -rf $(TARGET) *.o

.PHONY: all clean


run: $(TARGET)
	./$(TARGET)
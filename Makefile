
ARCH ?= $(shell uname -m)


CC := g++
CFLAGS := -Wall -g -fPIC
TARGET := bench


ifeq ($(ARCH), arm64)
ifeq ($(shell uname -i),x86_64)
CC = aarch64-linux-gnu-g++
endif
endif


M5_TARGET := bench-gem5


DIR = m5lib
LIB_SRC = m5op.S m5op_addr.S m5_mmap.c
_SRC = $(patsubst %,$(DIR)/%,$(LIB_SRC))
_OBJ = $(_SRC:.S=.o)
LIBOBJ = $(_OBJ:.c=.o)

$(DIR)/%.o : $(DIR)/%.S
		$(CC) -o $@ $< -c -g -fPIC -DM5OP_ADDR=0xFFFF0000 -I $(DIR)/




# $(wildcard *.cpp /xxx/xxx/*.cpp): get all .cpp files from the current directory and dir "/xxx/xxx/"
CPPFILES := $(wildcard *.cc)
SFILES := $(wildcard  *.S)
OBJS := $(CPPFILES:.cc=.o) $(SFILES:.S=.o) $(_OBJ:.c=.o)


CPPFILES := main.cc benchmark.cc
# CCASMFILES := singel_stride.cc

# OBJS := perf.o main.o benchmark.o single_stride.o





$(TARGET): $(OBJS)
	$(CC) -g -no-pie -static $^ -o $@

%.o : %.S
		$(CC) -o $@ $< -c -g

%.o : %.cc
		$(CC) -o $@ $< -c -g


# single_stride.o: single_stride.cc single_stride.S
# 	$(CC) -o $@ $< -c -g


all: $(TARGET)

clean: libclean
	rm -rf $(TARGET) *.o

.PHONY: all clean


run: $(TARGET)
	./$(TARGET)



libclean:
		rm -f $(DIR)/*.o
		rm -f $(DIR)/*.a

# m5ops: $(LIBOBJ)
# 		ar rc -o $(DIR)/lib$@.a $^
# 		ranlib $(DIR)/lib$@.a


N_BRANCHES := $$((1*1024))
N_BRANCHES := $$((512))


scramble_btb.h: scramble_btb.py
	python3 $< $(N_BRANCHES) False > $@

scramble_btb.c: scramble_btb.py
	python3 $< $(N_BRANCHES) main > $@

scramble_btb: scramble_btb.c
	clang -g -flto -O3 -Wall -Wextra $< -o $@


branch_btb_scramble.cc: scramble_btb.py
	python3 $< $(N_BRANCHES) branch_btb > $@
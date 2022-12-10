#name of output file
NAME = mtcap
#build dir
BD = ./build

#Linker flags
LDLIBS += 
LDDIRS += -L$(BD)

#Compiler flags
# -w to remove warnings
CFLAGS += -Wall -O2
I += -I./ -I/usr/include
LIB += -lpthread

#Compiler
CC = gcc -ggdb
AR = ar

#SRC=$(wildcard *.c)
SRC_BIN := main.c
SRC := $(SRC_LIB) $(SRC_BIN)
OBJS := $(SRC:%.c=$(BD)/%.o)
OBJS_LIB := $(SRC_LIB:%.c=$(BD)/%.o)

all: $(NAME) static shared
.PHONY: all clean

build_dir:
	mkdir -p $(BD)

$(NAME): $(OBJS) 
		$(CC) $(CFLAGS) $(I) $(LDDIRS) $(LDLIBS) $(LIB) $^ -o build/$(NAME)

$(BD)/%.o: %.c build_dir
		$(CC) $(CFLAGS) $(I) $(LDDIRS) $(LDLIBS) $< -c -o $@

shared: 
		$(CC) $(CFLAGS) $(I) $(LDDIRS) $(LDLIBS) $(SRC_BIN) -L./build $(LIB) -o $(BD)/$(NAME)_shared

static: 
		$(CC) $(CFLAGS) $(I) $(LDDIRS) $(LDDIRS) $(SRC_BIN) $(LIB) -static -o $(BD)/$(NAME)_static

clean:
		rm -rf $(BD)/*
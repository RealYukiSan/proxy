# Target executable
TARGET := main

# Source files
SOURCES := main.c ../src/core/HttpService.c ../src/config.c ../src/utils/string.c ../src/packet/packet.c

# Object files (derived from sources)
OBJECTS := $(SOURCES:.c=.o)

# Compiler and flags
CC := gcc
CFLAGS := -I ../include/ -I ../thirdparty/tlse -DVERBOSE -ggdb3
LDFLAGS := -L ../build/ -lEnet -lTlse

# Default rule (build the target)
all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	cp ../config.conf .

# Rule to compile source files into object files (implicit rule for .c to .o)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to clean up (remove object files and the target executable)
clean:
	rm -f $(OBJECTS) $(TARGET) config.conf
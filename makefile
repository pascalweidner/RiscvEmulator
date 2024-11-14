# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -g

# Source files
SRCS = main.c bus.c cpu.c dram.c machine.c
OBJS = $(SRCS:.c=.o)

# Target executable name
TARGET = program

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
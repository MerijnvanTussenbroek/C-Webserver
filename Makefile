#mingw32-make
#mingw32-make clean

# Compiler
CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -Werror

# Source and Object Files
SRC := $(wildcard src/*.c)
OBJ := $(SRC:.c=.o)

# Output executable
TARGET = server

# Add -lws2_32 to the linker flags
LDFLAGS = -lws2_32

# Default build
all: $(TARGET)

# Linking step
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)
	rm -f $(OBJ)  # Remove object files after linking

# Compilation step for each .c file
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean compiled files (including the executable)
clean:
	del /q src\*.o $(TARGET)

.PHONY: clean

CC = gcc
CFLAGS = -Wall -Wextra -O2 -Iinclude
SRC = $(wildcard src/*.c)
OBJDIR = build
OBJ = $(patsubst src/%.c,$(OBJDIR)/%.o,$(SRC))
TARGET = run_shell

# Compile .c -> .o in build/
$(OBJDIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Link all .o -> executable
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

clean:
	rm -rf $(OBJDIR) $(TARGET)

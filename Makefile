# compiler and flags
CC := gcc
CFLAGS := -O2 -I$(abspath include) # add here eventual compilation flags

# absolute paths
ROOT := $(abspath .)
SRC_DIR := $(ROOT)/src
OBJDIR := $(ROOT)/build
TARGET := $(ROOT)/run_shell

# source and object files
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(patsubst $(SRC_DIR)/%.c,$(OBJDIR)/%.o,$(SRC))

# compile .c -> .o in build/
$(OBJDIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# link all .o -> executable
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

.PHONY: clean
clean:
	rm -rf $(OBJDIR) $(TARGET)

# Makefile to compile .s files to .o files

# Define a variable for the assembler
AS = aarch64-linux-gnu-as
LD = aarch64-linux-gnu-ld

# Get a list of all .s files in the current directory
SRC = $(wildcard *.s)
# Define the corresponding .o files
OBJ = $(SRC:.s=.o)
EXEC = arm64

# The default target
all: comp run

comp: $(EXEC)
run:
	./$(EXEC)

$(EXEC): $(OBJ)
	$(LD) -o $@ $^

# Rule to create .o files from .s files
%.o: %.s
	$(AS) -o $@ $<

clean:
	rm -f $(OBJ) $(EXEC)

.PHONY: all clean run
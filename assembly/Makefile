# NOTE: this file is only responsible for compiling and running assembly code
# C++ code is not dealt with here (but may call this file to compile assembly code)

AS = aarch64-linux-gnu-as
LD = aarch64-linux-gnu-ld
STAGING_DIR = staging

SRC = $(wildcard $(STAGING_DIR)/**/*.s) $(wildcard $(STAGING_DIR)/*.s)

OBJ = $(SRC:.s=.o)
EXEC = arm64

all: stage call_comp run

stage: 
	./stage

# This lets us use our updated SRC and OBJ variables, since otherwise, these are evaluated before stage is run
call_comp:
	$(MAKE) comp --no-print-directory

comp: $(EXEC)

run:
	./$(EXEC)

$(EXEC): $(OBJ)
	$(LD) -o $@ $^

%.o: %.s
	$(AS) -o $@ $<

clean:
	rm -rf staging
	rm -f $(EXEC)

# runs the program in debug mode, need another terminal to run gdb
debug: $(EXEC)
	qemu-aarch64 -L /usr/aarch64-linux-gnu/ -g 1234 ./$(EXEC)

# see above
gdb: $(EXEC)
	gdb-multiarch -q --nh -ex 'set architecture arm64' -ex 'file $(EXEC)' -ex 'target remote localhost:1234' -ex 'layout split' -ex 'layout regs' 

.PHONY: all stage clean run debug gdb
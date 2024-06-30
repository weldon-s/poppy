AS = aarch64-linux-gnu-as
LD = aarch64-linux-gnu-ld

SRC = $(wildcard *.s)

OBJ = $(SRC:.s=.o)
EXEC = arm64

all: comp run

comp: $(EXEC)
run:
	./$(EXEC)

$(EXEC): $(OBJ)
	$(LD) -o $@ $^

%.o: %.s
	$(AS) -o $@ $<

clean:
	rm -f $(OBJ) $(EXEC)

# runs the program in debug mode, need another terminal to run gdb
debug: $(EXEC)
	qemu-aarch64 -L /usr/aarch64-linux-gnu/ -g 1234 ./$(EXEC)

# see above
gdb: $(EXEC)
	gdb-multiarch -q --nh -ex 'set architecture arm64' -ex 'file $(EXEC)' -ex 'target remote localhost:1234' -ex 'layout split' -ex 'layout regs' 

.PHONY: all clean run debug gdb
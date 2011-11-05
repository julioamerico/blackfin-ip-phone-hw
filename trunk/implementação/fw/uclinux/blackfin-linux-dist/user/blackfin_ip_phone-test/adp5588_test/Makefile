CFLAGS ?= -g -O2
CFLAGS += -Wall
FLTFLAGS += -s 0x2000

all: adp5588_test

romfs:
	$(ROMFSINST) /bin/adp5588_test

clean:
	rm -f adp5588_test *.elf *.gdb *.o

.PHONY: all clean romfs

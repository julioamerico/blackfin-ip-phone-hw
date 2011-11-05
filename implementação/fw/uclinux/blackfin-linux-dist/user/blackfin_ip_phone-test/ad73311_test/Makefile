CFLAGS ?= -O2 -pipe
CFLAGS += -Wall

all: ad73311_test

romfs:
	$(ROMFSINST) /bin/ad73311_test
	$(ROMFSINST) /seno_ip_phone.wav
	$(ROMFSINST) /sample.wav

clean:
	rm -f ad73311_test *.elf *.gdb *.o

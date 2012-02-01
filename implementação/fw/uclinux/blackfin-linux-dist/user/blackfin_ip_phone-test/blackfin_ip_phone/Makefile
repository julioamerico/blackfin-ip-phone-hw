EXEC = main
OBJS = main.o

CC = bfin-uclinux-gcc

all: $(EXEC)

$(EXEC): $(OBJS) dep_files
	$(CC) $(LDFLAGS) -o $@ $(OBJS) fsm/queue.o fsm/fsm.o $(LDLIBS$(LDLIBS_$@))

make_fsm:
	(cd /fsm;$(MAKE))

romfs:
	$(ROMFSINST) /bin/$(EXEC)

dep_files:
	true

clean:
	-rm -f $(EXEC) *.elf *.gdb *.o
 

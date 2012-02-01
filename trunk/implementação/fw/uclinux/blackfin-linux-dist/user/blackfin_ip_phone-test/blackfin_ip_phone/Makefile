EXEC = main
OBJS = main.o
SUBDIR = ./fsm

all: $(EXEC)

$(EXEC): $(OBJS) make_fsm
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(SUBDIR)/queue.o $(SUBDIR)/fsm.o $(LDLIBS$(LDLIBS_$@))

make_fsm:
	(cd $(SUBDIR);$(MAKE))

romfs:
	$(ROMFSINST) /bin/$(EXEC)

clean:
	-rm -f $(EXEC) *.elf *.gdb *.o
	cd $(SUBDIR); $(MAKE) clean;

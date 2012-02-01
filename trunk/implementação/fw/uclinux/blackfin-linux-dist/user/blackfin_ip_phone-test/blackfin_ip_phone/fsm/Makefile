fsm.o: fsm.c fsm.h queue.h queue.o
	$(CC) $(LDFLAGS) -c fsm.c $(LDLIBS$(LDLIBS_$@))

queue.o: queue.c fsm.h queue.h
	$(CC) $(LDFLAGS) -c queue.c $(LDLIBS$(LDLIBS_$@))

clean:
	-rm -f *.elf *.gdb *.o

IDIR = -I../ipphone -I../ipphone/linphone/linphone-3.1.2/coreapi

fsm.o: fsm.c fsm.h queue.h queue.o
	$(CC) $(LDFLAGS) -c fsm.c $(IDIR) $(LDLIBS$(LDLIBS_$@))

queue.o: queue.c fsm.h queue.h
	$(CC) $(LDFLAGS) -c queue.c $(LDLIBS$(LDLIBS_$@))

clean:
	-rm -f *.elf *.gdb *.o

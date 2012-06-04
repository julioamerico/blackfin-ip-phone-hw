IDIR = -I./linphone/linphone-3.1.2/coreapi -I../fsm

ipphone.o: ipphone.c Linphone
	$(CC) $(LDFLAGS) -c ipphone.c $(IDIR)

Linphone:
	@(cd ./linphone;$(MAKE))

clean:
	-rm -f main *.elf *.gdb *.o
		
		


 




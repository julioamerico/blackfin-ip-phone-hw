PATH_LIB = $(ROOTDIR)/staging/usr/lib
LIBS = linphone ortp mediastreamer m asound speex speexdsp pthread rt dl osipparser2 osip2 eXosip2
IDIR = -I./ipphone -I./ipphone/linphone/linphone-3.1.2/coreapi

LIBS_LIST = $(patsubst %,-l%, $(LIBS))

EXEC = main
OBJS = main.o

all: $(EXEC)

$(EXEC): $(OBJS) make_fsm make_drv_lcd make_drv_expander make_ipphone
	$(CC) $(LDFLAGS) -o $@ $(OBJS) ./fsm/queue.o ./fsm/fsm.o ./drivers/lcd/drv_lcd.o ./drivers/lcd/lcd.o ./drivers/event/expander.o ./ipphone/ipphone.o -L$(PATH_LIB) $(LIBS_LIST) $(LDLIBS$(LDLIBS_$@))

main.o: main.c make_ipphone
	$(CC) $(LDFLAGS) -c main.c $(IDIR) 

make_fsm: make_ipphone
	(cd ./fsm;$(MAKE))
	
make_drv_lcd: make_ipphone
	(cd ./drivers/lcd;$(MAKE))

make_drv_expander:
	(cd ./drivers/event;$(MAKE))

make_ipphone:
	(cd ./ipphone;$(MAKE))

romfs:
	$(ROMFSINST) /bin/$(EXEC)
	#$(ROMFSINST) -d ./ipphone/main /usr/bin/main

clean:
	-rm -f $(EXEC) *.elf *.gdb *.o
	cd ./fsm; $(MAKE) clean;
	cd ./drivers/lcd; $(MAKE) clean;
	cd ./drivers/event; $(MAKE) clean;
	cd ./ipphone/; $(MAKE) clean;

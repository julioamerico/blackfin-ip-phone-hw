EXEC = main
OBJS = main.o

all: $(EXEC)

$(EXEC): $(OBJS) make_fsm make_drv_lcd make_drv_expander
	$(CC) $(LDFLAGS) -o $@ $(OBJS) ./fsm/queue.o ./fsm/fsm.o ./drivers/lcd/drv_lcd.o ./drivers/lcd/lcd.o ./drivers/event/expander.o $(LDLIBS$(LDLIBS_$@))

make_fsm:
	(cd ./fsm;$(MAKE))
	
make_drv_lcd:
	(cd ./drivers/lcd;$(MAKE))

make_drv_expander:
	(cd ./drivers/event;$(MAKE))

romfs:
	$(ROMFSINST) /bin/$(EXEC)

clean:
	-rm -f $(EXEC) *.elf *.gdb *.o
	cd ./fsm; $(MAKE) clean;
	cd ./drivers/lcd; $(MAKE) clean;
	cd ./drivers/event; $(MAKE) clean;

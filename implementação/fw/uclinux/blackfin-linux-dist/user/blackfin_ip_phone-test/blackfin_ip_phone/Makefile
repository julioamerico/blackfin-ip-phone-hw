EXEC = main
OBJS = main.o

all: $(EXEC)

$(EXEC): $(OBJS) make_fsm make_drv_lcd
	$(CC) $(LDFLAGS) -o $@ $(OBJS) ./fsm/queue.o ./fsm/fsm.o ./drivers/lcd/drv_lcd.o ./drivers/lcd/lcd.o $(LDLIBS$(LDLIBS_$@))

make_fsm:
	(cd ./fsm;$(MAKE))
	
make_drv_lcd:
	(cd ./drivers/lcd;$(MAKE))

romfs:
	$(ROMFSINST) /bin/$(EXEC)

clean:
	-rm -f $(EXEC) *.elf *.gdb *.o
	cd ./fsm; $(MAKE) clean;
	cd ./drivers/lcd; $(MAKE) clean;

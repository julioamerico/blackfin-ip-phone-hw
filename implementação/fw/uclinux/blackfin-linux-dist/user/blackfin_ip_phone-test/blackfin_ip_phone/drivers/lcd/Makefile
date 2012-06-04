IDIR = -I../../ipphone -I../../ipphone/linphone/linphone-3.1.2/coreapi

drv_lcd.o: drv_lcd.c drv_lcd.h linux/i2c-dev.h lcd.o
	$(CC) $(LDFLAGS) -c drv_lcd.c $(LDLIBS$(LDLIBS_$@))

lcd.o: lcd.c drv_lcd.h lcd.h
	$(CC) $(LDFLAGS) -c lcd.c $(IDIR) $(LDLIBS$(LDLIBS_$@))

clean:
	-rm -f *.elf *.gdb *.o

dir_y = 
dir_n =
dir_  =
dir_all = $(dir_y) $(dir_n) $(dir_)

dir_$(CONFIG_USER_BLACKFIN_IP_PHONE_AD73311_TEST)  += ad73311_test
dir_$(CONFIG_USER_BLACKFIN_IP_PHONE_ADP5588_TEST)  += adp5588_test
dir_$(CONFIG_USER_BLACKFIN_IP_PHONE_MAIN_APP)      += blackfin_ip_phone
dir_$(CONFIG_USER_BLACKFIN_IP_PHONE_LCD_TEST)      += lcd_test

all romfs:
	for i in $(dir_y)   ; do $(MAKE) -C $$i $@ || exit $$? ; done

clean:
	for i in $(dir_all) ; do $(MAKE) -C $$i $@ || exit $$? ; done

.PHONY: all clean romfs

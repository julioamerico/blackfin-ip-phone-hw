/*
 * U-boot - main board file
 *
 * Copyright (c) 2008-2009 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#include <common.h>
#include <config.h>
#include <command.h>
#include <net.h>
#include <netdev.h>
#include <spi.h>
#include <asm/blackfin.h>
#include <asm/net.h>
#include <asm/portmux.h>
#include <asm/mach-common/bits/otp.h>
#include <asm/sdh.h>

DECLARE_GLOBAL_DATA_PTR;

int checkboard(void)
{
	printf("Board: Blackfin IP-Phone board\n");
	printf("       Please, visit http://code.google.com/p/blackfin-ip-phone-hw\n");
	return 0;
}

#if defined(CONFIG_BFIN_MAC)
int board_eth_init(bd_t *bis)
{
	return bfin_EMAC_initialize(bis);
}
#endif

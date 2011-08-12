/*
 * U-boot - Configuration file for Blackfin IP Phone Board
 */

#ifndef __CONFIG_BF518_IPPHONE_H__
#define __CONFIG_BF518_IPPHONE_H__

#include <asm/config-pre.h>


/*
 * Processor Settings
 */
#define CONFIG_BFIN_CPU             bf518-0.1
#define CONFIG_BFIN_BOOT_MODE       BFIN_BOOT_SPI_MASTER

/*
#define CONFIG_DEBUG_SERIAL
#define CONFIG_DEBUG_EARLY_SERIAL
#define DEBUG
*/


/*
 * Clock Settings
 *	CCLK = (CLKIN * VCO_MULT) / CCLK_DIV
 *	SCLK = (CLKIN * VCO_MULT) / SCLK_DIV
 */
/* CONFIG_CLKIN_HZ is any value in Hz					*/
#define CONFIG_CLKIN_HZ			25000000
/* CLKIN_HALF controls the DF bit in PLL_CTL      0 = CLKIN		*/
/*                                                1 = CLKIN / 2		*/
#define CONFIG_CLKIN_HALF		0
/* PLL_BYPASS controls the BYPASS bit in PLL_CTL  0 = do not bypass	*/
/*                                                1 = bypass PLL	*/
#define CONFIG_PLL_BYPASS		0
/* VCO_MULT controls the MSEL (multiplier) bits in PLL_CTL		*/
/* Values can range from 0-63 (where 0 means 64)			*/
#define CONFIG_VCO_MULT			16
/* CCLK_DIV controls the core clock divider				*/
/* Values can be 1, 2, 4, or 8 ONLY					*/
#define CONFIG_CCLK_DIV			1
/* SCLK_DIV controls the system clock divider				*/
/* Values can range from 1-15						*/
#define CONFIG_SCLK_DIV			4


/*
 * Memory Settings
 */
/* This board has a MT48LC16M16A2P-75 */
#define CONFIG_MEM_ADD_WDTH	9
#define CONFIG_MEM_SIZE		32

#define CONFIG_EBIU_SDRRC_VAL	0x0305
#define CONFIG_EBIU_SDGCTL_VAL	0x8091118D
#define CONFIG_EBIU_SDBCTL_VAL  0x0013

#define CONFIG_EBIU_AMGCTL_VAL	(AMCKEN | AMBEN_ALL)
#define CONFIG_EBIU_AMBCTL0_VAL	(B1WAT_15 | B1RAT_15 | B1HT_3 | B1RDYPOL | B0WAT_15 | B0RAT_15 | B0HT_3 | B0RDYPOL)
#define CONFIG_EBIU_AMBCTL1_VAL	(B3WAT_15 | B3RAT_15 | B3HT_3 | B3RDYPOL | B2WAT_15 | B2RAT_15 | B2HT_3 | B2RDYPOL)

#define CONFIG_SYS_MONITOR_LEN	(512 * 1024)
#define CONFIG_SYS_MALLOC_LEN	(384 * 1024)


/*
 * Network Settings
 */
#if !defined(__ADSPBF512__) && !defined(__ADSPBF514__)
#define ADI_CMDS_NETWORK	1
#define CONFIG_BFIN_MAC
/*#define CONFIG_BFIN_MAC_PINS \
	{ \
	P_MII0_ETxD0, \
	P_MII0_ETxD1, \
	P_MII0_ETxD2, \
	P_MII0_ETxD3, \
	P_MII0_ETxEN, \
	P_MII0_TxCLK, \
	P_MII0_PHYINT, \
	P_MII0_COL, \
	P_MII0_ERxD0, \
	P_MII0_ERxD1, \
	P_MII0_ERxD2, \
	P_MII0_ERxD3, \
	P_MII0_ERxDV, \
	P_MII0_ERxCLK, \
	P_MII0_CRS, \
	P_MII0_MDC, \
	P_MII0_MDIO, \
	0 }*/
#define CONFIG_NETCONSOLE	1
#define CONFIG_NET_MULTI	1
#endif
#define CONFIG_HOSTNAME		bf518-ipphone
#define CONFIG_PHY_ADDR		1
/* Uncomment next line to use fixed MAC address */
#define CONFIG_ETHADDR	02:80:ad:20:31:e8

#define CONFIG_SYS_NO_FLASH


/*
 * SPI Settings
 */
#define CONFIG_BFIN_SPI
#define CONFIG_SPI_FLASH
#define CONFIG_CMD_SF
#define CONFIG_SF_DEFAULT_SPEED	50000000

#define CONFIG_ENV_IS_IN_SPI_FLASH
#define CONFIG_ENV_SPI_MAX_HZ	50000000
#define CONFIG_ENV_SPI_BUS  0
#define CONFIG_ENV_SPI_CS   2

#define CONFIG_SPI_FLASH_SPANSION


/*
 * Env Storage Settings
 */
#define CONFIG_ENV_IS_IN_SPI_FLASH
#define CONFIG_ENV_OFFSET	0x10000
#define CONFIG_ENV_SIZE		0x2000
#define CONFIG_ENV_SECT_SIZE	0x10000
#define CONFIG_ENV_IS_EMBEDDED_IN_LDR


/*
 * I2C Settings
 */
#define CONFIG_BFIN_TWI_I2C	1
#define CONFIG_HARD_I2C		1


/*
 * SDH Settings
 */
#if !defined(__ADSPBF512__)
#define CONFIG_GENERIC_MMC
#define CONFIG_MMC
#define CONFIG_BFIN_SDH
#endif


/*
 * Misc Settings
 */
#define CONFIG_RTC_BFIN

#define CONFIG_UART_CONSOLE	0
#define CONFIG_BAUDRATE    115200


/*
 * Pull in common ADI header for remaining command/environment setup
 */
#include <configs/bfin_adi_common.h>

#endif

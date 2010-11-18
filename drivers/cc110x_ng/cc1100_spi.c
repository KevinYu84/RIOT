/******************************************************************************
Copyright 2010, Freie Universitaet Berlin (FUB). All rights reserved.

These sources were developed at the Freie Universitaet Berlin, Computer Systems
and Telematics group (http://cst.mi.fu-berlin.de).
-------------------------------------------------------------------------------
This file is part of µkleos.

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

FeuerWare is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see http://www.gnu.org/licenses/ .
--------------------------------------------------------------------------------
For further information and questions please use the web site
	http://scatterweb.mi.fu-berlin.de
and the mailinglist (subscription via web site)
	scatterweb@lists.spline.inf.fu-berlin.de
*******************************************************************************/

/**
 * @ingroup		dev_cc110x
 * @{
 */

/**
 * @file
 * @internal
 * @brief		TI Chipcon CC1100 SPI driver
 *
 * @author      Freie Universität Berlin, Computer Systems & Telematics, FeuerWhere project
 * @author		Thomas Hillebrandt <hillebra@inf.fu-berlin.de>
 * @author		Heiko Will <hwill@inf.fu-berlin.de>
 * @version     $Revision: 1775 $
 *
 * @note		$Id: cc1100_spi.c 1775 2010-01-26 09:37:03Z hillebra $
 */

#include <stdio.h>

#include <cc1100.h>
#include <cc1100-arch.h>
#include <cc1100-internal.h>
#include <cc1100_spi.h>

#include <irq.h>

/*---------------------------------------------------------------------------*/
//					    CC1100 SPI access
/*---------------------------------------------------------------------------*/

#define NOBYTE 0xFF

uint8_t cc1100_spi_writeburst_reg(uint8_t addr, char *src, uint8_t count) {
	int i = 0;
	unsigned int cpsr = disableIRQ();
	cc1100_spi_select();
	cc1100_txrx(addr | CC1100_WRITE_BURST);
	while (i < count) {
		cc1100_txrx(src[i]);
		i++;
	}
	cc1100_spi_unselect();
	restoreIRQ(cpsr);
	return count;
}

void cc1100_spi_readburst_reg(uint8_t addr, char *buffer, uint8_t count) {
	int i = 0;
	unsigned int cpsr = disableIRQ();
	cc1100_spi_select();
	cc1100_txrx(addr | CC1100_READ_BURST);
	while (i < count) {
		buffer[i] = cc1100_txrx(NOBYTE);
		i++;
	}
	cc1100_spi_unselect();
	restoreIRQ(cpsr);
}

void cc1100_spi_write_reg(uint8_t addr, uint8_t value) {
	unsigned int cpsr = disableIRQ();
	cc1100_spi_select();
	cc1100_txrx(addr);
	cc1100_txrx(value);
	cc1100_spi_unselect();
	restoreIRQ(cpsr);
}

uint8_t cc1100_spi_read_reg(uint8_t addr) {
	uint8_t result;
	unsigned int cpsr = disableIRQ();
	cc1100_spi_select();
	cc1100_txrx(addr | CC1100_READ_SINGLE);
	result = cc1100_txrx(NOBYTE);
	cc1100_spi_unselect();
	restoreIRQ(cpsr);
	return result;
}

uint8_t cc1100_spi_read_status(uint8_t addr) {
	uint8_t result;
	unsigned int cpsr = disableIRQ();
	cc1100_spi_select();
	cc1100_txrx(addr | CC1100_READ_BURST);
	result = cc1100_txrx(NOBYTE);
	cc1100_spi_unselect();
	restoreIRQ(cpsr);
	return result;
}

uint8_t cc1100_spi_strobe(uint8_t c) {
	uint8_t result;
	unsigned int cpsr = disableIRQ();
	cc1100_spi_select();
	result = cc1100_txrx(c);
	cc1100_spi_unselect();
	restoreIRQ(cpsr);
	return result;
}

/** @} */

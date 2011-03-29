//********************************************************************************************
//
// File : enc28j60.c Microchip ENC28J60 Ethernet Interface Driver
//
//********************************************************************************************
//
// Copyright (C) 2007
//
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 2 of the License, or (at your option) any later
// version.
// This program is distributed in the hope that it will be useful, but
//
// WITHOUT ANY WARRANTY;
//
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc., 51
// Franklin St, Fifth Floor, Boston, MA 02110, USA
//
// http://www.gnu.de/gpl-ger.html
//
//********************************************************************************************
#include "includes.h"
//
//#define F_CPU 8000000UL  // 8 MHz

//struct enc28j60_flag
//{
//	unsigned rx_buffer_is_free:1;
//	unsigned unuse:7;
//}enc28j60_flag;
static BYTE Enc28j60Bank;
static WORD_BYTES next_packet_ptr;

//*******************************************************************************************
//
// Function : icmp_send_request
// Description : Send ARP request packet to destination.
//
//*******************************************************************************************
BYTE enc28j60ReadOp(BYTE op, BYTE address)
{
	return(0);
}
//*******************************************************************************************
//
// Function : icmp_send_request
// Description : Send ARP request packet to destination.
//
//*******************************************************************************************
void enc28j60WriteOp(BYTE op, BYTE address, BYTE datap)
{
}
//*******************************************************************************************
//
// Function : icmp_send_request
// Description : Send ARP request packet to destination.
//
//*******************************************************************************************
void enc28j60SetBank(BYTE address)
{
}
//*******************************************************************************************
//
// Function : icmp_send_request
// Description : Send ARP request packet to destination.
//
//*******************************************************************************************
BYTE enc28j60Read(BYTE address)
{
	// select bank to read
	enc28j60SetBank(address);
	
	// do the read
	return enc28j60ReadOp(ENC28J60_READ_CTRL_REG, address);
}
//*******************************************************************************************
//
// Function : icmp_send_request
// Description : Send ARP request packet to destination.
//
//*******************************************************************************************
void enc28j60Write(BYTE address, BYTE datap)
{
}
//*******************************************************************************************
//
// Function : icmp_send_request
// Description : Send ARP request packet to destination.
//
//*******************************************************************************************
WORD enc28j60_read_phyreg(BYTE address)
{
	return 0;
}
//*******************************************************************************************
//
// Function : icmp_send_request
// Description : Send ARP request packet to destination.
//
//*******************************************************************************************
void enc28j60PhyWrite(BYTE address, WORD datap)
{
}
//*******************************************************************************************
//
// Function : icmp_send_request
// Description : Send ARP request packet to destination.
//
//*******************************************************************************************
/*
void enc28j60_init( BYTE *avr_mac)
{
}
*/
void enc28j60_init( BYTE *avr_mac)
{
}
//*******************************************************************************************
//
// Function : enc28j60getrev
// Description : read the revision of the chip.
//
//*******************************************************************************************
BYTE enc28j60getrev(void)
{
	return(0);
}
//*******************************************************************************************
//
// Function : enc28j60_packet_send
// Description : Send packet to network.
//
//*******************************************************************************************
void enc28j60_packet_send ( BYTE *buffer, WORD length )
{
}
//*******************************************************************************************
//
// Function : enc28j60_mac_is_linked
// Description : return MAC link status.
//
/*******************************************************************************************
/*
BYTE enc28j60_mac_is_linked(void)
{
	if ( (enc28j60_read_phyreg(PHSTAT1) & PHSTAT1_LLSTAT ) )
		return 1;
	else
		return 0;
}
*/
//*******************************************************************************************
//
// Function : enc28j60_packet_receive
// Description : check received packet and return length of data
//
//*******************************************************************************************
//WORD data_length;
WORD enc28j60_packet_receive ( BYTE *rxtx_buffer, WORD max_length )
{
	return( 0 );
}


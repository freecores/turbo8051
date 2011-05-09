//********************************************************************************************
//
// YagnaInnWebServer firmware 
//
// Author(s) : Dinesh Annayya, dinesha@opencores.org   
// Website   : http://www.yagnainn.com/
// MCU       : Open Core 8051 @ 50Mhz
// Version   : 1.0
//********************************************************************************************
//
// File : ethDriver.c Ethernet Driver for Yagna Innovation -WebBrowser development board.
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
unsigned int iRxFrmCnt = 0;
unsigned int iTxFrmCnt = 0;
unsigned int iRxDescPtr= 0;
unsigned int iTxDescPtr= 0;

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
void enc28j60_packet_send ( XBYTE *buffer, WORD length )
{
    WORD_BYTES iRxFrmStatus, data_length;
   __xdata __at (0xA030) unsigned int iMacRxFrmCnt;
   XDWORD *pTxDesPtr;
   __xdata unsigned int  tDataPtr; // Temp DataPointer

    pTxDesPtr = (XDWORD *) (0x7040 | iTxDescPtr);
    //*pTxDesPtr = (length & 0xFFF) ;
    *pTxDesPtr = (XDWORD ) buffer ;
    *pTxDesPtr = (*pTxDesPtr >> 2); // Aligned 32 bit addressing
    *pTxDesPtr = (*pTxDesPtr << 12); // Move to Address Position
    *pTxDesPtr |= (length & 0xFFF); 
//    *pTxDesPtr |= ((buffer << 12) & 0x3FFF000);
    iTxDescPtr = (iTxDescPtr+4) & 0x3F;
    iTxFrmCnt  = iRxFrmCnt+1;

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
// Function : ethPakcetReceive
// Description : check received packet and return length of data
//
//*******************************************************************************************
//WORD data_length;
WORD enc28j60_packet_receive ( BYTE *rxtx_buffer, WORD max_length )
{
    WORD_BYTES iRxFrmStatus, data_length;
   __xdata __at (0xA030) unsigned int iMacRxFrmCnt;
   __xdata unsigned long *pRxDesPtr;


    // check if a packet has been received and buffered
    if((iMacRxFrmCnt & 0xF) != 0) { // Check the Rx Q Counter
       pRxDesPtr = (__xdata unsigned long *) (0x7000 | iRxDescPtr);
       data_length.word = *pRxDesPtr & 0xFFF; // Last 12 bit indicate the Length of the Packet
       rxtx_buffer = ((*pRxDesPtr >> 12) & 0x3FFF) << 2 ; // 32 bit Aligned Address
       iRxFrmStatus.word= *pRxDesPtr >> 26; // Upper 6 Bit Inidcate the Rx Status
       iRxDescPtr = (iRxDescPtr+4) & 0x3F;
       iRxFrmCnt  = iRxFrmCnt+1;

    }
    if ( data_length.word > (max_length-1) )
    {
      data_length.word= max_length-1;
    }
    return( data_length.word );
}


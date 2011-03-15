// Copyright (C) 2002 Mason Kidd (mrkidd@nettaxi.com)
//
// This file is part of MicroWeb.
//
// MicroWeb is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// MicroWeb is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with MicroWeb; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

// ip.c: IP protocol processing

#include <string.h>
#include "packets.h"
#include "csio.h"

unsigned char rx_ip_packet(unsigned char *rx_buffer)
{
	struct eth_hdr *rx_eth_hdr = (struct eth_hdr *)rx_buffer;
	struct ip_hdr *rx_ip_hdr = (struct ip_hdr *)(rx_buffer + sizeof(struct eth_hdr));
	unsigned int *chksum_hdr = (unsigned int *)rx_ip_hdr;
	int i;
	unsigned int chksum = 0;
	
	// Make sure that the packet is destined for me or for broadcast
	if ((!memcmp(myIP, &rx_ip_hdr->destIP, sizeof(unsigned char) * 4)) || (!memcmp(myBCAST, &rx_ip_hdr->destIP, sizeof(unsigned char) * 4)))
	{
       	// Save the source MAC and IP for when I reply
       	memcpy(srcMAC, &rx_eth_hdr->shost, sizeof(unsigned char) * 6);
       	memcpy(srcIP, &rx_ip_hdr->srcIP, sizeof(unsigned char) * 4);
		
		// Compute the checksum
		for (i = 0; i < 10; i++)
			if (i != 5)
				chksum += *chksum_hdr;
		chksum = ~chksum;
 		
 		// packet is valid if they match
 		if (chksum == rx_ip_hdr->hdrchksum)
  		{
			return rx_ip_hdr->proto;
    	}
     	else
      		return 0;
	}
	return 0;
}

void tx_ip_packet(unsigned char *tx_buffer, unsigned char tx_length)
{
	struct eth_hdr *tx_eth_hdr = (struct eth_hdr *)tx_buffer;
	struct ip_hdr *tx_ip_hdr = (struct ip_hdr *)(tx_buffer + sizeof(struct eth_hdr));
	unsigned int *chksum_hdr = (unsigned int *)tx_ip_hdr;
	int i;
	unsigned int chksum = 0;

    tx_ip_hdr->verIHL = 0x45;
    tx_ip_hdr->totlen = tx_length + sizeof(struct ip_hdr);
    tx_ip_hdr->TTL = 0x32;
    tx_ip_hdr->proto = IP_UDP;
    
	memcpy(tx_ip_hdr->srcIP, myIP, sizeof(unsigned char) * 4);
	memcpy(tx_ip_hdr->destIP, targetIP, sizeof(unsigned char) * 4);
	
	chksum_hdr = (unsigned int *)tx_ip_hdr;
	// Compute the checksum
	for (i = 0; i < 10; i++)
		if (i != 5)
			chksum += *chksum_hdr;
	chksum = ~chksum;
	tx_ip_hdr->hdrchksum = chksum;

	tx_eth_hdr->type = ETHER_IP;
	memcpy(tx_eth_hdr->shost, myMAC, sizeof(unsigned char) * 6);
	memcpy(tx_eth_hdr->dhost, targetMAC, sizeof(unsigned char) * 6);
	
   	tx_packet(tx_buffer, tx_length);
}


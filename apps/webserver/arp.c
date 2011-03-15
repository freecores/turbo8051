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

// arp.c: processing for ARP packets

#include <string.h>
#include "packets.h"
#include "csio.h"
#include "arp.h"

void tx_arp_packet(unsigned int arp_oper)
{
	unsigned char tx_buf[BUF_LEN];
	struct eth_hdr *tx_eth_hdr = (struct eth_hdr *)(tx_buf);
	struct eth_arp *tx_eth_arp = (struct eth_arp *)(tx_buf + sizeof(struct eth_hdr));
	unsigned int nLength = 0;

	tx_eth_arp->eth_arp_hdr.ar_hrd = ARP_HRD_ETHER;
	tx_eth_arp->eth_arp_hdr.ar_pro = ETHER_IP;
	tx_eth_arp->eth_arp_hdr.ar_hln = MAC_ADDR_LEN;
	tx_eth_arp->eth_arp_hdr.ar_pln = IP_ADDR_LEN;
	tx_eth_arp->eth_arp_hdr.ar_op = arp_oper;

	memcpy(tx_eth_arp->ar_sha, myMAC, sizeof(char) * MAC_ADDR_LEN);
	memcpy(tx_eth_arp->ar_spa, myIP, sizeof(char) * IP_ADDR_LEN);
	memcpy(tx_eth_arp->ar_tha, targetMAC, sizeof(char) * MAC_ADDR_LEN);
	memcpy(tx_eth_arp->ar_tpa, targetIP, sizeof(char) * IP_ADDR_LEN);

	memcpy(tx_eth_hdr->dhost, targetMAC, sizeof(char) * MAC_ADDR_LEN);
	memcpy(tx_eth_hdr->shost, myMAC, sizeof(char) * MAC_ADDR_LEN);
	tx_eth_hdr->type = ETHER_ARP;

	nLength += sizeof(struct eth_hdr) + sizeof(struct eth_arp);

	tx_packet(tx_buf, nLength);
}

void rx_arp_packet(unsigned char *rx_buffer)
{
	struct eth_hdr *rx_eth_hdr = (struct eth_hdr *)rx_buffer;
	struct eth_arp *rx_eth_arp = (struct eth_arp *)(rx_buffer + sizeof(struct eth_hdr));
	
	// make sure the ARP packet is Ethernet and IP
	if ((rx_eth_arp->eth_arp_hdr.ar_hrd == ARP_HRD_ETHER) && (rx_eth_arp->eth_arp_hdr.ar_pro == ETHER_IP))
	{
		// make sure the ARP packet is a request destined for us
		if ((rx_eth_arp->eth_arp_hdr.ar_op == ARP_REQUEST) && (!memcmp(myIP, rx_eth_arp->ar_tpa, sizeof(unsigned char) * 4)))
		{
			// send a reply
			memcpy(targetMAC, rx_eth_arp->ar_sha, sizeof(char) * MAC_ADDR_LEN);
			memcpy(targetIP, rx_eth_arp->ar_spa, sizeof(char) * IP_ADDR_LEN);
			tx_arp_packet(ARP_REPLY);
		}
       		// else discard packet
	}
	// else discard packet
}


#
#
#

SHELL = /bin/sh
SDCCFLAGS = --model-large -I /home/dinesha/download/sdcc/device/include/mcs51 -I /home/dinesha/download/sdcc/device/include/  -L /home/dinesha/download/sdcc/device/lib/build/large
ASLINKFLAGS = --code-loc 0x8000 --data-loc 0x30 --stack-loc 0x20 --xram-loc 0xD000
MODULES = main.rel menu.rel ip.rel lcd.rel tcp.rel udp.rel icmp.rel http.rel ethernet.rel arp.rel enc28j60.rel

%.rel: %.c
	sdcc $(SDCCFLAGS) -c $<

#main.rel: main.c
#	sdcc $(SDCCFLAGS) -c $<

all: turbo8051.hex

turbo8051.hex :: $(MODULES)
	sdcc $(SDCCFLAGS) $(ASLINKFLAGS) $(MODULES)
	packihx main.ihx > turbo8051.hex

clean:
	rm -f *.asm *.hex *.ihx *.lnk *.lst *.map *.rel *.rst *.sym *.mem


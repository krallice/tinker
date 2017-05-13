CC=gcc

all:
	$(CC) tinker.c iface.c iptable.c transtable.c arp.c -o tinker

clean:
	rm -rf tinker

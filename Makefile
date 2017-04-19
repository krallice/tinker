CC=gcc

all:
	$(CC) tinker.c iptable.c transtable.c -o tinker

clean:
	rm -rf tinker

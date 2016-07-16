CC=gcc

all:
	$(CC) tinker.c -o tinker

clean:
	rm -rf tinker

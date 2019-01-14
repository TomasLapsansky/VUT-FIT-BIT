CC = gcc
CFLAGS = -Wall -Wextra -Werror -Ilib/ -g
PCAPFLAG = -lpcap

all: myripsniffer myripresponse

bin/print.o: src/print.c lib/print.h
	$(CC) src/print.c $(CFLAGS) -c -o bin/print.o $(PCAPFLAG)

bin/sniffer.o: src/sniffer.c lib/sniffer.h lib/print.h
	$(CC) src/sniffer.c $(CFLAGS) -c -o bin/sniffer.o $(PCAPFLAG)

bin/myripsniffer.o: src/myripsniffer.c lib/sniffer.h lib/print.h
	$(CC) src/myripsniffer.c $(CFLAGS) -c -o bin/myripsniffer.o $(PCAPFLAG)

myripsniffer: bin/myripsniffer.o bin/sniffer.o bin/print.o
	$(CC) bin/myripsniffer.o bin/sniffer.o bin/print.o $(CFLAGS) -o myripsniffer $(PCAPFLAG)

bin/myripresponse.o: src/myripresponse.c lib/response.h
	$(CC) src/myripresponse.c $(CFLAGS) -c -o bin/myripresponse.o $(PCAPFLAG)

bin/response.o: src/response.c lib/response.h lib/print.h
	$(CC) src/response.c $(CFLAGS) -c -o bin/response.o $(PCAPFLAG)

myripresponse: bin/myripresponse.o bin/response.o bin/print.o
	$(CC) bin/myripresponse.o bin/response.o bin/print.o $(CFLAGS) -o myripresponse $(PCAPFLAG)

clean:
	rm bin/* myripsniffer myripresponse

test-sniffer: myripsniffer
	sudo ./myripsniffer -i ens33

test-response: myripresponse
	sudo ./myripresponse -i ens33 -r 2001:db8:0:abcd::/64

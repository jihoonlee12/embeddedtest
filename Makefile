CC = arm-linux-gnueabi-gcc
AR = arm-linux-gnueabi-ar

all: libMyperi.a lcdtexttest

libMyperi.a: lcdtext.o fnd.o buzzer.o button.o led.o
	$(AR) rc libMyperi.a lcdtext.o fnd.o buzzer.o button.o led.o
	
led.o: led.h led.c
	$(CC) led.c -o led.o -c

button.o: button.h button.c
	$(CC) button.c -o button.o -c

buzzer.o: buzzer.h buzzer.c
	$(CC) buzzer.c -o buzzer.o -c
	
fnd.o: fnd.h fnd.c
	$(CC) fnd.c -o fnd.o -c
	
fndtest: fndtest.c fnd.h libMyperi.a
	$(CC) fndtest.c -o fndtest -l Myperi -L.
	
lcdtext.o: lcdtext.h lcdtext.c
	$(CC) lcdtext.c -o lcdtext.o -c
	
lcdtexttest: lcdtexttest.c lcdtext.h libMyperi.a
	$(CC) lcdtexttest.c -o lcdtexttest -l Myperi -L.
	
clean:
	rm *.o -rf
	rm *.a -rf
	rm lcdtexttest -rf

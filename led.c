#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>

#include "led.h"

static int fd=0;
static unsigned int ledValue=0;
int ledLibInit(void)	//이 라이브러리는 led를 켜기 전에 반드시 한번 불러주어야 한다.
{
	fd=open(LED_DRIVER_NAME, O_RDWR);
	if(fd <0){
		printf("Device open Error!\r\n");
		exit(1);
	}
	ledValue=0;
	ledLibRaw(ledValue);
	return 1;
}
int ledOnOff (int ledNum, int onOff)	
{//이  함수는 ledNum에는 0~7을 넣으면, 그에 해단하는 led를 켜겨나 끄거나 해준다.
	int lamp=1;
	lamp=lamp<<ledNum;	//00010000 해당하는 전구비트만 1임
	ledValue=ledValue&(~lamp);	//해당하는 전구비트만 0으로 바꿈.
	if(onOff !=0) ledValue |=lamp;	//해당하는 전구비트만 1로 바꾸고 나머지는 그대로.
	ledLibRaw(ledValue);
	return 1;
}
int ledStatus (void)
{//이 함수를 호출하면 현재 LED상태가 어떤지 확인해 준다. 만약 모든 LED가 켜져 있으면 0xff를 리턴하고,
//만약 모든 LED가 꺼져 있으면, 0x00을 리턴한다.
//만약 0xAA가 리턴되면, (10101010)->LED는 4개만 켜져있는 것이다.
	return ledValue;
}
int ledLibExit(void)
{//이 함수를 호출하면 모든 LED를 전부 끄고 driver를 close한다.
	ledLibRaw(0);	//전부 끔
	close(fd);
	return 1;
}
int ledLibRaw(char ledstatus)
{//이 함수를 호출하면 ledstatus에 맞게 8개의 led를 전부 제어한다. ex) ledstatus(0x80); 1개만 켜진다.
	if(fd>0){	//do Something
		write(fd, &ledstatus, 1);
	}
	else{
		printf("WRONG! you must open device NODE!\r\n");
		exit(2);
	}
	ledValue = ledstatus;
	return 1;
}
int ledLibBlink(int ledNum, int nth, int msec)
{//1~8개 까지의 LED를 하나 골라서 n번 깜빡이는데, 주기가 msec
	ledLibMorseBlink(ledNum, nth, msec, msec);
}
int ledLibMorseBlink(int ledNum, int nth, int msecOn, int msecOff)
{//1~8개 까지의 LED를 하나 골라서 n번 깜빡이는데, 켜져있는 시간이 msecOn, 꺼져있는 시간이 msecOff.
	for(int i=0;i<nth;i++){
		ledOnOff(ledNum, 1);	//ON
		usleep(1000*msecOn);
		ledOnOff(ledNum, 0);	//OFF
		usleep(1000*msecOff);
	}
	return 1;
}

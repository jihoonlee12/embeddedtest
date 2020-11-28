#ifndef _LED_H_
#define _LED_H_
int ledLibInit(void);	//이 라이브러리는 led를 켜기 전에 반드시 한번 불러주어야 한다.
int ledOnOff (int ledNum, int onOff);	
//이  함수는 ledNum에는 0~7을 넣으면, 그에 해단하는 led를 켜겨나 끄거나 해준다.
int ledStatus (void);
//이 함수를 호출하면 현재 LED상태가 어떤지 확인해 준다. 만약 모든 LED가 켜져 있으면 0xff를 리턴하고,
//만약 모든 LED가 꺼져 있으면, 0x00을 리턴한다.
//만약 0xAA가 리턴되면, (10101010)->LED는 4개만 켜져있는 것이다.
int ledLibExit(void);
//이 함수를 호출하면 모든 LED를 전부 끄고 driver를 close한다.
int ledLibRaw(char ledstatus);
//이 함수를 호출하면 ledstatus에 맞게 8개의 led를 전부 제어한다. ex) ledstatus(0x80); 1개만 켜진다.
int ledLibBlink(int ledNum, int nth, int msec);
//1~8개 까지의 LED를 하나 골라서 n번 깜빡이는데, 주기가 msec
int ledLibMorseBlink(int ledNum, int nth, int msecOn, int msecOff);
//1~8개 까지의 LED를 하나 골라서 n번 깜빡이는데, 켜져있는 시간이 msecOn, 꺼져있는 시간이 msecOff.
#define LED_DRIVER_NAME "/dev/periled"

#endif _LED_H_

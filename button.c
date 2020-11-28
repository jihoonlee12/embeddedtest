#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
#include <unistd.h>	//for open/close
#include <fcntl.h>		//for O_RDWR
#include <sys/ioctl.h>	//for ioctl
#include <sys/msg.h>
#include <pthread.h>
#include "button.h"

//first read input device
#define INPUT_DEVICE_LIST "/dev/input/event"
//실제 디바이스 드라이버 노드파일: 뒤에 숫자가 붙음, ex)/dev/input/event5
#define PROBE_FILE "/proc/bus/input/devices"
//PPT에 제시된 "이 파일을 까보면 event?의 숫자를 알 수 있다"는 바로 그 파일
#define HAVE_TO_FIND_1 "N: Name=\"ecube-button\"\n"
#define HAVE_TO_FIND_2 "H: Handlers=kbd event"

pthread_t buttonTh_id;

static int fd=0;
static int msgID;
typedef struct{
	struct timeval time;
	__u16 type;
	__u16 code;
	__s32 value;
} input_event;

int probeButtonPath(char *newPath)
{
	FILE *fp;
	int returnValue = 0;	//button에 해당하는 event#을 찾았나?
	int number =0;				//찾았다면 여기에 집어넣자
	if((fp = fopen(PROBE_FILE,"rt"))==NULL)	//파일을 열고
	{ 
		printf("PROBE_FILE open fail\r\n");
		exit(2);
	}
	while(!feof(fp))	//끝까지 읽어들인다.
	{
		char tmpStr[200];	//200자를 읽을 수 있게 버퍼
		fgets(tmpStr, 200, fp);	//최대 200자를 읽어봄
		//printf("%s",tmpStr);
		if(strcmp(tmpStr, HAVE_TO_FIND_1)==0)
		{
			printf("YES! I found!: %s\r\n", tmpStr);
			returnValue =1;	//찾음
		}
		if(returnValue ==1 &&	//찾은 상태에서
			(strncasecmp(tmpStr, HAVE_TO_FIND_2, strlen(HAVE_TO_FIND_2))==0))	//event#을 찾았으면
			{
				printf("-->%s",tmpStr);
				printf("\t%c\r\n",tmpStr[strlen(tmpStr)-3]);
				number = tmpStr[strlen(tmpStr)-3] - '0';
				//Ascii character '0'-'9' (0x30 - 0x39)
				//to interger(0)
				break;
			}
	}
	fclose(fp);
	if(returnValue == 1)
	sprintf(newPath, "%s%d", INPUT_DEVICE_LIST, number);
	return returnValue;
}

void* buttonThFunc(void *arg)
{
	static int retval = 1;
	int readSize;
	struct input_event stEvent;
	BUTTON_MSG_T messageTxData;
	while(1)
	{
		readSize = read(fd, &stEvent, sizeof(stEvent));
		if(readSize != sizeof(stEvent))
		{
			continue;
		}
		if(stEvent.type == EV_KEY)
		{
			switch(stEvent.code)
			{
				case KEY_VOLUMEUP: messageTxData.keyInput=1;
					break;
				case KEY_HOME: messageTxData.keyInput=2; 
					break;
				case KEY_SEARCH: messageTxData.keyInput=3; 
					break;
				case KEY_BACK: messageTxData.keyInput=4;
					break;
				case KEY_MENU: messageTxData.keyInput=5;
					break;
				case KEY_VOLUMEDOWN: messageTxData.keyInput=6;
					break;
			}
			if( stEvent.value) messageTxData.pressed = 1;
			else messageTxData.pressed = 0;
			msgsnd(msgID, &messageTxData, sizeof(messageTxData.keyInput)+sizeof(messageTxData.pressed),0);
		}	//End of if
		else  //EV_SYN
			;	//do nothing
		if(messageTxData.keyInput == 4) pthread_exit((void*)&retval);	//back 버튼을 누를 시 스레드 종료
	}	// End of While
}

int buttonInit(void)
{
	char buttonPath[200]={0,};
	if(probeButtonPath(buttonPath) == 0)
	{
		printf("Error! File not found\r\n");
		exit(0);
	}
	fd=open(buttonPath, O_RDONLY);
	if(fd <0){
		printf("Device open Error!\r\n");
		exit(1);
	}
	msgID = msgget(MESSAGE_ID, IPC_CREAT|0666);
	if(msgID==-1)
	{
		printf("Cannot get msgID\r\n");
		return 1;
	}
	int err = pthread_create(&buttonTh_id, NULL, &buttonThFunc, NULL);
	if(err!=0)
		printf("Thread Create Error\r\n");
	return msgID;
}

int buttonExit(void)
{
	void *tret = NULL;
	pthread_join (buttonTh_id, &tret);
	close(fd);
	printf("Good Bye~\n");
	return 1;
}

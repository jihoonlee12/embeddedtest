#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

#include "buzzer.h"

#define MAX_SCALE_STEP 8
#define BUZZER_BASE_SYS_PATH  "/sys/bus/platform/devices/"
#define BUZZER_FILENAME "peribuzzer"
#define BUZZER_ENABLE_NAME "enable"
#define BUZZER_FREQUENCY_NAME "frequency"

char gBuzzerBaseSysDir[128]; ///sys/bus/platform/devices/peribuzzer.XX 가 결정됨
const int musicScale[MAX_SCALE_STEP] =
{
	262, /*do*/ 294,330,349,392,440,494, /* si */ 523
};



static int fd = 0;


int findBuzzerSysPath(void)
{
	DIR* dir_info = opendir(BUZZER_BASE_SYS_PATH);
	int ifNotFound = 1;
	if (dir_info != NULL)
	{
		while (1)
		{
			struct dirent *dir_entry;
			dir_entry = readdir (dir_info);
			if (dir_entry == NULL) break;
			if (strncasecmp(BUZZER_FILENAME, dir_entry->d_name, strlen(BUZZER_FILENAME)) == 0)
			{
				ifNotFound = 0;
				sprintf(gBuzzerBaseSysDir,"%s%s/",BUZZER_BASE_SYS_PATH, dir_entry->d_name);
			}
		}
	}
	printf("find %s\n",gBuzzerBaseSysDir);
	return ifNotFound;
}

void buzzerEnable(int bEnable)
{
	char path[200];
	sprintf(path,"%s%s",gBuzzerBaseSysDir,BUZZER_ENABLE_NAME);
	fd=open(path,O_WRONLY);
	if ( bEnable) write(fd, &"1", 1);
	else write(fd, &"0", 1);
	close(fd);
}

void setFrequency(int frequency)
{
	char path[200];
	sprintf(path,"%s%s",gBuzzerBaseSysDir,BUZZER_FREQUENCY_NAME);
	fd=open(path,O_WRONLY);
	dprintf(fd, "%d", frequency);
	close(fd);
}

int buzzerInit(void)		// 이 라이브러리는 부저를 켜기 전에 반드시 한번 불러주어야 한다
{
	if(findBuzzerSysPath() == 1)
	{
		printf("Error! File not found\r\n");
		exit(0);
	}
}

int buzzerPlaySong(int scale)	//  음계 (1: 도, 2:레,... etc) 를 입력받아 Buzzer를 울리는 함수
{
	// 262, /*do*/ 294,330,349,392,440,494, /* si */ 523
	buzzerEnable(1);
	switch(scale)
			{
				case 0 : buzzerStopSong();
					break;
				case 1 : setFrequency(262);
					break;
				case 2 : setFrequency(294); 
					break;
				case 3 : setFrequency(330); 
					break;
				case 4 : setFrequency(349);
					break;
				case 5 : setFrequency(392);
					break;
				case 6 : setFrequency(440);
					break;
				case 7 : setFrequency(494);
					break;
				case 8 : setFrequency(523);
					break;
			}
	return 1;
}
int buzzerStopSong(void)				// Buzzer 소리를 죽이는 함수
{
	buzzerEnable(0);
	return 1;
}

int buzzerExit(void)				// 이 함수는 부저를 전부 끄고 driver를 close한다.
{
	buzzerStopSong();
	printf("Good Bye~\n");
	return 1;
}

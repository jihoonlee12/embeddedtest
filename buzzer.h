#ifndef _BUZZER_H_
#define _BUZZER_H_
int buzzerInit(void);		// 이 라이브러리는 부저를 켜기 전에 반드시 한번 불러주어야 한다
int buzzerPlaySong(int scale);	//  이 함수는  부저를 켜준다.
int buzzerStopSong(void);				// 이 함수는 부저를 멈춘다.
int buzzerExit(void);						// 이 함수는 부저를 전부 끄고 driver를 close한다.
#endif

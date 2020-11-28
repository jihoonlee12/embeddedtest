#include <stdio.h>
#include <stdlib.h>
#include "fnd.h"

int main(void)
{
	printf("123456, all dots turn on\n");
	fndDisp(111111, 63);
	sleep(5);
	printf("999999, all dots turn off\n");
	fndDisp(999999, 0);
	sleep(5);
	return 0;
}



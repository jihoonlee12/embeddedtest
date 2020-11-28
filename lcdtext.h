#ifndef _LCDTEXT_H_
#define _LCDTEXT_H_

#define LINE_NUM 2
#define COLUMN_NUM 16
typedef struct TextLCD_tag
{
unsigned char cmd; //
unsigned char cmdData;
unsigned char reserved[2];
char TextData[LINE_NUM][LINE_BUFF_NUM];
}stTextLCD,*pStTextLCD;

int lcdtextwrite(const char *str1, const char *str2, int lineflag);
#endif

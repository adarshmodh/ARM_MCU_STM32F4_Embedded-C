#define _lcd_
#include "Lcd.h"
#include "PORT.h"


void InitializeLCD(void)
{
	WriteLCDCommand(0x38);
  WriteLCDCommand(0x01);
  WriteLCDCommand(0x06);
  WriteLCDCommand(0x0c);
  WriteLCDCommand(0x80);
}
	
	
void ready(void)
{
	uint8_t i;
	for(i=0;i<=50;i++)
		delay(1000);
}

void clearLCD(void)
{
	WriteLCDCommand(0x01);
}

void WriteLCDCommand(uint8_t temp)
{
	ready();
	DATA_Port->ODR&=~((uint32_t)DATA_Pin);
	DATA_Port->ODR|=temp<<(DATA_Pin_0-1);
		
	RS_Port->BSRRH=(uint16_t)RS_Pin;
	delay(1000);
	RWB_Port->BSRRH=(uint16_t)RWB_Pin;
	delay(1000);
	EN_Port->BSRRL=(uint16_t)EN_Pin;
	delay(1000);
	EN_Port->BSRRH=(uint16_t)EN_Pin;
}

void WriteLCDData(uint8_t temp)
{
	ready();
	DATA_Port->ODR&=~((uint32_t)DATA_Pin);
	DATA_Port->ODR|=temp<<(DATA_Pin_0-1);
	RS_Port->BSRRL=(uint16_t)RS_Pin;
	delay(1000);
	RWB_Port->BSRRH=(uint16_t)RWB_Pin;
	delay(1000);
	EN_Port->BSRRL=(uint16_t)EN_Pin;
	delay(1000);
	EN_Port->BSRRH=(uint16_t)EN_Pin;
}
void WriteLCDLine1(uint8_t *LnPtr1)
{
	unsigned char c1;
    WriteLCDCommand(LCD_LINE1_CHAR_ADD);
    for(c1=0 ; c1<LCD_LINE_LENGTH ; c1++)
        WriteLCDData(*LnPtr1++);
}
void WriteLCDLine2(uint8_t *LnPtr1)
{
	unsigned char c1;
    WriteLCDCommand(LCD_LINE2_CHAR_ADD);
    for(c1=0 ; c1<LCD_LINE_LENGTH ; c1++)
        WriteLCDData(*LnPtr1++);
}
void GoToXY(uint8_t x,uint8_t y)
{
	if((y>1)&&(x>15))
		return;
	switch(y)
	{
		case 0:
			WriteLCDCommand(LCD_LINE1_CHAR_ADD+x);
			break;
		case 1:
			WriteLCDCommand(LCD_LINE2_CHAR_ADD+x);
			break;
	}
}

void delay(uint32_t sec)
{
	//100000000 ~ 2sec
	while(sec--)
	{}	
}	


void displayvalue(unsigned int a)
{
	unsigned int c[10];
	int i=0;

	while(a!=0)
		{
			c[i] = a%10;
			a=a/10;
			i++;
		}
		
	i=i-1;
	WriteLCDCommand(LCD_LINE2_CHAR_ADD);
		
	while(i>=0)
	{
		WriteLCDData(c[i]+48);
		i--;
	}	
}

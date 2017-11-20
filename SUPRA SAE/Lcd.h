#include "stm32f4xx.h"

#ifdef _lcd_

	void InitializeLCD(void);
	void ready(void);
	void WriteLCDCommand(uint8_t);
	void WriteLCDData(uint8_t);
	void WriteLCDLine1(uint8_t *);
	void WriteLCDLine2(uint8_t *);
	void GoToXY(uint8_t,uint8_t);
	void delay(uint32_t sec);
	void clearLCD(void);	
	void displayvalue(unsigned int a);
	unsigned char*tempdata;
	
#else

	extern void InitializeLCD(void);
	extern void ready(void);
	extern void WriteLCDCommand(uint8_t);
	extern void WriteLCDData(uint8_t);
	extern void WriteLCDLine1(uint8_t *);
	extern void WriteLCDLine2(uint8_t *);
	extern void GoToXY(uint8_t,uint8_t);
	extern void delay(uint32_t sec);	
	extern void clearLCD(void);
	extern void displayvalue(unsigned int a);
	extern unsigned char*tempdata;
#endif




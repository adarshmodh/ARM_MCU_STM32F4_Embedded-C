#include "stm32f4xx.h"

#ifdef _lcd_

	void InitializeLCD(void);
	void ready(void);
	void WriteLCDCommand(uint8_t);
	void WriteLCDData(uint8_t);
	void WriteLCDLine1(uint8_t *);
	void WriteLCDLine2(uint8_t *);
	void GoToXY(uint8_t,uint8_t);
	void delay(uint32_t );
	void clearLCD(void);	
	void displayvalue(unsigned int );
	unsigned char*tempdata;
	
#else

	extern void InitializeLCD(void);
	extern void ready(void);
	extern void WriteLCDCommand(uint8_t);
	extern void WriteLCDData(uint8_t);
	extern void WriteLCDLine1(uint8_t *);
	extern void WriteLCDLine2(uint8_t *);
	extern void GoToXY(uint8_t,uint8_t);
	extern void delay(uint32_t );	
	extern void clearLCD(void);
	extern void displayvalue(unsigned int );
	extern unsigned char*tempdata;
#endif




#include "stm32f4xx.h"

GPIO_InitTypeDef  GPIO_InitStructure;

uint16_t i;

void delay(uint16_t sec)
{
	while(sec--)
	{}	
}	

void LCD_cmd(uint8_t temp)
{	
	for(i=0; i<=10000; i++)
	{}
	GPIOD->ODR = temp;
	for(i=0; i<=10000; i++)
	{}
	GPIOB -> BSRRH = GPIO_Pin_9;
	for(i=0; i<=10000; i++)
	{}
	GPIOA -> BSRRH = GPIO_Pin_8;
	for(i=0; i<=10000; i++)
	{}
	GPIOB -> BSRRL = GPIO_Pin_2;
	for(i=0; i<=10000; i++)
	{}
	GPIOB -> BSRRH = GPIO_Pin_2;
	for(i=0; i<=10000; i++)
	{}
}

void LCD_data(uint8_t temp)
{	
	for(i=0; i<=10000; i++)
	{}
	GPIOD->ODR = temp;
	for(i=0; i<=10000; i++)
	{}
	GPIOB -> BSRRL = GPIO_Pin_9;
	for(i=0; i<=10000; i++)
	{}
	GPIOA -> BSRRH = GPIO_Pin_8;
	for(i=0; i<=10000; i++)
	{}
	GPIOB -> BSRRL = GPIO_Pin_2;
	for(i=0; i<=10000; i++)
	{}
	GPIOB -> BSRRH = GPIO_Pin_2;

}

void LCD_init()
{
	LCD_cmd(0x38);
	LCD_cmd(0x06);
	LCD_cmd(0x01);
}

int main(void)
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_5| GPIO_Pin_6| GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_9 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	  GPIOA->BSRRH=(uint16_t)GPIO_Pin_8;
		
	LCD_init();
	LCD_data('a');
	
  
	while (1)
  {	
		
	}
}

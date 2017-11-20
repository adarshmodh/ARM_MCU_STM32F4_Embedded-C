#include "stm32f4xx.h"

TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

void TIM_Config(void);
void keyscan(void);

uint16_t PrescalerValue = 0;
uint16_t flag = 0;
uint16_t counter = 0;
uint16_t keystatus = 0;
uint16_t oldstatus = 0x04;

int main(void)
{
  TIM_Config();

  PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 4000) - 1;

  TIM_TimeBaseStructure.TIM_Period = 399;
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  TIM_Cmd(TIM3, ENABLE);

  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

  while (1)
  {
		if(flag==1)
		{
			flag=0;
			keyscan();
		}
	}
}


void TIM_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
   
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOD, &GPIO_InitStructure); 
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOE, &GPIO_InitStructure);   
  
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
void TIM3_IRQHandler(void)
{
  
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update );
    flag = 1;
  }
}
void keyscan()
{
		
	keystatus = (GPIOE->IDR & GPIO_Pin_2);
	if(keystatus!=oldstatus)
	{
		oldstatus = keystatus;
		counter++;
		if(counter == 2)
		{
			counter = 0;
			GPIOD -> ODR ^= GPIO_Pin_12;
		}
	}
}
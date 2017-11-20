#include "stm32f4xx.h"

uint16_t flag = 0;

uint16_t TimerPeriod = 0;

GPIO_InitTypeDef GPIO_InitStructure;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
NVIC_InitTypeDef NVIC_InitStructure;
	
void GPIO_Config(void);
void TIM1_Config(void);
void TIM2_Config(void);

int main(void)
{
  
  GPIO_Config();
	TIM1_Config();
	TIM2_Config();
  
while(1)
{
		if(flag==1)
		{
			flag=0;
			GPIOD -> ODR ^= GPIO_Pin_12;
		}
		else if(flag==2)
		{
			flag=0;
			GPIOD -> ODR ^= GPIO_Pin_13;
		}
}
}

void TIM1_CC_IRQHandler(void)
{
  
  if (TIM_GetITStatus(TIM1, TIM_IT_CC2) != RESET)
  {
    TIM_ClearITPendingBit(TIM1, TIM_IT_CC2 );
    flag = 1;
  }
}

void TIM2_IRQHandler(void)
{
  
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update );
    flag=2;
  }
}

 void GPIO_Config(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
  
	//timer1 1sec delay|timer2 sec delay
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 |GPIO_Pin_13 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
}

void TIM1_Config(void)
{
	  
  /* Time Base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) ((SystemCoreClock) / (4000)) - 1;// 1sec

  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 3999;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  /* TIM enable counter */
  TIM_Cmd(TIM1, ENABLE);

  /* Enable the CC2 Interrupt Request */
  TIM_ITConfig(TIM1, TIM_IT_CC2, ENABLE);
	
		
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}

void TIM2_Config(void)
{
	  
  /* Time Base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) ((SystemCoreClock/2) / (2000)) - 1;// 2sec

  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 3999;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  TIM_Cmd(TIM2, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
		
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}



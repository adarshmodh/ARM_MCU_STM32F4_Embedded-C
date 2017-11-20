#include "stm32f4xx.h"

int flag = 0, k;
uint16_t TimerPeriod = 0;

GPIO_InitTypeDef GPIO_InitStructure;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
NVIC_InitTypeDef NVIC_InitStructure;
	
void GPIO_Config(void);
void TIM3_Config(void);


int main(void)
{
	GPIO_Config();	
  TIM3_Config();

  
  while (1)
  {
		k = 0X1000;
		if(flag==1)
		{
			GPIOD -> ODR = k;
		}
		else if(flag==2)
		{
			flag = 0;
			k = k<<1;
			GPIOD -> ODR = k;
		}
	
	}
}

void TIM3_IRQHandler(void)
{
  
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update );
    flag ++;
  }
}

void TIM3_Config(void)
{
	  
  /* Time Base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) ((SystemCoreClock/2) / (500000)) - 1;// 5000Hz

  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 99;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  TIM_Cmd(TIM3, ENABLE);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
		
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}


void GPIO_Config(void)
{	
		
  /* GPIOA, GPIOB and GPIOE Clocks enable */
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD , ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
  
 // output
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12| GPIO_Pin_13 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
 
}


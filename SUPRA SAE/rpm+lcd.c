
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "Lcd.h"
#include "PORT.h"

__IO uint16_t IC3ReadValue1 = 0, IC3ReadValue2 = 0;
__IO uint16_t CaptureNumber = 0;
__IO uint32_t Capture = 0;
__IO uint32_t TIM1Freq = 0;
__IO uint32_t RPM = 0;

unsigned int ovf = 0;


GPIO_InitTypeDef GPIO_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
TIM_ICInitTypeDef  TIM_ICInitStructure;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
uint16_t TimerPeriod = 0;
uint16_t Channel1Pulse = 0, Channel2Pulse = 0, Channel3Pulse = 0, Channel4Pulse = 0;

void GPIO_config(void);
void TIM1_Config(void);//input capture

	
int main(void)
{

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);//inputcapture
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); //neutral, temp, inputcapture
 
	
	GPIO_config();
	TIM1_Config();
	
	InitializeLCD();
	
  while (1)
	{
	
		
		//neutral indicator
		if(((GPIOE->IDR)& GPIO_Pin_2) != GPIO_Pin_2)
		GPIOD->BSRRL = GPIO_Pin_12;
		else if(((GPIOE->IDR)& GPIO_Pin_2) == GPIO_Pin_2)
		GPIOD->BSRRH = GPIO_Pin_12;
		
	}
	
}


void TIM1_CC_IRQHandler(void)
{ 
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
		{
			TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    
				if(CaptureNumber == 1)
				{
				ovf++; 		
				
				}
				
		}
  
	if(TIM_GetITStatus(TIM1, TIM_IT_CC2) == SET) 
  {
    /* Clear TIM1 Capture compare interrupt pending bit */
    TIM_ClearITPendingBit(TIM1, TIM_IT_CC2);
    if(CaptureNumber == 0)
    {
      /* Get the Input Capture value */
      IC3ReadValue1 = TIM_GetCapture2(TIM1);
      CaptureNumber = 1;
    }
    else if(CaptureNumber == 1)
    {
      /* Get the Input Capture value */
      IC3ReadValue2 = TIM_GetCapture2(TIM1); 
      
      /* Capture computation */
      if (IC3ReadValue2 > IC3ReadValue1)
      {
         Capture = (IC3ReadValue2 - IC3ReadValue1); 
      }
      else if (IC3ReadValue2 < IC3ReadValue1)
      {
        Capture = ((0xFFFF - IC3ReadValue1) + IC3ReadValue2) + (ovf)*(0xFFFF); 
      }
      else
      {
        Capture = 0;
      }
      /* Frequency computation */ 
      TIM1Freq = (uint32_t) SystemCoreClock / Capture/100;
			RPM = TIM1Freq*20;
			CaptureNumber = 0;
			ovf=0;
			
			/*if(RPM < y)
				GPIOD->ODR = GPIO_Pin_13;
				else if((RPM > y) && (RPM < z))
				GPIOD->ODR = GPIO_Pin_14;	
				else if(RPM > z)
				GPIOD->ODR = GPIO_Pin_15;
				else
				GPIOD->ODR = 0x0000;
			*/
			
			
			//LCD display
			clearLCD(); 
			WriteLCDLine1("RPM              ");
			displayvalue(TIM1Freq);
			
		}
  }
}


void GPIO_config()
{
	
  
  
	////input capture//////
	 
  /* TIM1 channel 2 pin (PE.11) configuration */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  /* Connect TIM pins to AF2 */
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);
  
  /* Enable the TIM1 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  	
  
	
	////for LCD//////
	
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
	
	
	////for indicator LEDs
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	/// 12-N, 13,14,15-> BGR
	
	/// sensor inputs
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
	// 2-neutral
	
}


void TIM1_Config(void)
{
  /////for input capture//////
	
  TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;

  TIM_ICInit(TIM1, &TIM_ICInitStructure);
  
	TIM_TimeBaseStructure.TIM_Prescaler = 99;

  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 65535;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
  /* TIM enable counter */
  TIM_Cmd(TIM1, ENABLE);

  /* Enable the CC2 Interrupt Request */
  TIM_ITConfig(TIM1, TIM_IT_CC2, ENABLE);
	// ovf update
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
}



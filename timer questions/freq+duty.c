#include "stm32f4xx.h"

__IO uint16_t IC3ReadValue1 = 0, IC3ReadValue2 = 0, IC3ReadValue3=0;
__IO uint16_t CaptureNumber = 0;
unsigned int ovf = 0,ovfcount=0, flag=0,pinstatus;
__IO uint32_t Capture1 = 0,Capture2 = 0;
__IO uint32_t TIM4Freq = 0;
__IO uint32_t RPM = 0;

float dutycycle;

TIM_ICInitTypeDef  TIM_ICInitStructure;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
uint16_t TimerPeriod = 0;
uint16_t Channel1Pulse = 0, Channel2Pulse = 0, Channel3Pulse = 0, Channel4Pulse = 0;


void GPIO_config(void);
void TIM4_Config(void);//input capture
void TIM3_Config(void);//waveform

int main(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//waveform
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//inputcapture
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_config();
	TIM3_Config();
	TIM4_Config();
	
  while (1)
{
}
}

void TIM4_IRQHandler(void)
{ 
		if(CaptureNumber==0)
		{
			if(((GPIOB->IDR)& GPIO_Pin_7) != GPIO_Pin_7)
			pinstatus=0;
			else if(((GPIOB->IDR)& GPIO_Pin_7) == GPIO_Pin_7)
			pinstatus=1;
		}
		
		if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)
		{
			TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    
				if(CaptureNumber >= 1)
				{
				ovf++; 		
				}
				
		}
  
	if(TIM_GetITStatus(TIM4, TIM_IT_CC2) == SET) 
  {
    /* Clear TIM4 Capture compare interrupt pending bit */
    TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);
    
		
		if(CaptureNumber == 0)
    {
      /* Get the Input Capture value */
      IC3ReadValue1 = TIM_GetCapture2(TIM4);
      CaptureNumber = 1;
    }
    else if(CaptureNumber == 1)
    {
      /* Get the Input Capture value */
      IC3ReadValue2 = TIM_GetCapture2(TIM4); 
			
			/* Capture computation */
      if (IC3ReadValue2 >= IC3ReadValue1)
      {
        Capture1 = IC3ReadValue2 - IC3ReadValue1; 
      }
      else if (IC3ReadValue2 < IC3ReadValue1)
      {
        Capture1 = ((0xFFFF - IC3ReadValue1) + IC3ReadValue2) ; 
      }
      
			/* Frequency computation */ 
      
			ovfcount = ovf;	
			Capture1 = Capture1 + ovfcount*0xFFFF;
			ovf=0;
			CaptureNumber=2;
		}
		else if(CaptureNumber==2)
		{	
			/* Get the Input Capture value */
       
			IC3ReadValue3 = TIM_GetCapture2(TIM4);
      
			/* Capture computation */
      if (IC3ReadValue3 >= IC3ReadValue2)
      {
        Capture2 = IC3ReadValue3 - IC3ReadValue2; 
      }
      else if (IC3ReadValue3 < IC3ReadValue2)
      {
        Capture2 = ((0xFFFF - IC3ReadValue2) + IC3ReadValue3) ; 
      }
      
			/* Frequency computation */ 
      
			ovfcount = ovf;	
			Capture2 = Capture2 + ovfcount*0xFFFF;
			ovf=0;	
			
			if(pinstatus==1)
			{
				dutycycle = (float) (Capture1)/(Capture1 + Capture2);
			}	
			else if(pinstatus==0)
			{
				dutycycle = (float) (Capture2)/(Capture1 + Capture2);
			}	
			
			CaptureNumber = 0;
		}	
		}
  }



void GPIO_config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
 
	////input capture//////
	 
  /* TIM4 channel 2 pin (PB.7) configuration */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Connect TIM pins to AF2 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);
  
  /* Enable the TIM4 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  	 
	
	////for waveform generation////
	 
  /* GPIOA Configuration: Channel 1, 3 and 4 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
  
}

void TIM3_Config(void)
{	
	/////waveform/////
	
	/* Time Base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) ((SystemCoreClock/2) / 100000) - 1; //1Khz

  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 999;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* Channel 1, 2,3 and 4 Configuration in PWM mode */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 50;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);

  /* TIM3 counter enable */
  TIM_Cmd(TIM3, ENABLE);

  /* TIM3 Main Output Enable */
  TIM_CtrlPWMOutputs(TIM3, ENABLE);
}

void TIM4_Config(void)
{
  /////for input capture//////
	
  TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;

  TIM_ICInit(TIM4, &TIM_ICInitStructure);
  
	TIM_TimeBaseStructure.TIM_Prescaler = 99;

  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 65535;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
  /* TIM enable counter */
  TIM_Cmd(TIM4, ENABLE);

  /* Enable the CC2 Interrupt Request */
  TIM_ITConfig(TIM4, TIM_IT_CC2, ENABLE);
	// ovf update
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
}



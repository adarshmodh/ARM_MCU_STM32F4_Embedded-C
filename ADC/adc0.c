#include "stm32f4xx.h"

__IO uint16_t ADC3ConvertedValue = 0;
__IO uint32_t ADC3ConvertedVoltage = 0;
uint16_t PrescalerValue = 0;
unsigned int voltage=0;

TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
ADC_InitTypeDef       ADC_InitStructure;

GPIO_InitTypeDef      GPIO_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;

void ADC3_config(void);
void GPIO_config(void);
void TIM3_config(void);

int main(void)
{
	 /* Enable ADC3, DMA2 and GPIO clocks ****************************************/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
	
	GPIO_config();
	TIM3_config();
	ADC3_config();
	
	TIM_Cmd(TIM3, ENABLE);
	ADC_Cmd(ADC3, ENABLE);
 

  while (1)
  {
  }
}

void ADC_IRQHandler(void)
{
	
	if (ADC_GetITStatus(ADC3, ADC_IT_EOC) != RESET)
	{
    ADC_ClearITPendingBit(ADC3, ADC_IT_EOC );
		voltage =(uint16_t)ADC3->DR;
		//voltage=voltage*3.3/4095;
	}
}	

void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update );
		/* Start ADC3 Software Conversion */ 
		ADC_SoftwareStartConv(ADC3);
	}
}	


void ADC3_config(void)
{
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  //ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC3, &ADC_InitStructure);

  ADC_RegularChannelConfig(ADC3, ADC_Channel_0, 1, ADC_SampleTime_3Cycles);
	
	ADC_ITConfig(ADC3, ADC_IT_EOC, ENABLE);
	
  NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void GPIO_config(void)
{	
  /* Configure ADC3 Channel0 pin as analog input ******************************/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, & GPIO_InitStructure);
 
}

void TIM3_config(void)
{
	  
  /* Time Base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) ((SystemCoreClock)/2 / (2000)) - 1;// 1ms delay

  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 1;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
	 /* Enable the TIM1 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}

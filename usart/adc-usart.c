#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include <stdio.h>

USART_InitTypeDef USART_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
ADC_InitTypeDef ADC_InitStructure;



__IO uint16_t ADC3ConvertedValue = 0;
__IO uint32_t ADC3ConvertedVoltage = 0;
uint16_t PrescalerValue = 0;
void TIM_Config(void);
void ADC3_Config(void);
void USART_init(void);
void GPIO_init(void);
unsigned int voltage=0;


#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

	
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */     
       
  /* USARTx configured as follow:
        - BaudRate = 19200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  
	
	//Tx Rx port clock enable
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD , ENABLE);
	
	//USART clock enable
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);  //ADC enable
	
	GPIO_init();
	ADC3_Config();
	USART_init();
	
	ADC_Cmd(ADC3, ENABLE);

  /* Output a message on Hyperterminal using printf function */
  printf("\n %d ", voltage);

  while (1)
  {
		
  }
}

void ADC_IRQHandler(void)
{

	if (ADC_GetITStatus(ADC3, ADC_IT_EOC) != RESET)
	{
    ADC_ClearITPendingBit(ADC3, ADC_IT_EOC );
		voltage = (uint16_t)ADC3->DR;
		voltage  = voltage*3.3/4095;
	}
}

void USART3_IRQHandler(void)
{
uint16_t Value;
USART_ClearITPendingBit(USART3,USART_IT_RXNE);
Value=USART_ReceiveData(USART3);
USART_SendData(USART3,Value-0x20);	
}

void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update );
		ADC_SoftwareStartConv(ADC3);
	}
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART3, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
  {}

  return ch;
}

void ADC3_Config(void)
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


void USART_init(void)
{	
	SystemCoreClockUpdate();
  USART_InitStructure.USART_BaudRate = 19200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
	
  /* USART configuration */
  USART_Init(USART3, &USART_InitStructure);
	
	/* Enable the USARTx Receive interrupt: this interrupt is generated when the
  USARTx receive data register is not empty */
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
	//receive interrupt enable
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	
	/* Enable USART */
  USART_Cmd(USART3, ENABLE);
	
}

void GPIO_init(void)
{
  /* Configure USART Tx & Rx as alternate function  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8| GPIO_Pin_9;//tx rx
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
		/* Connect PXx to USARTx_Tx*/
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);

  /* Connect PXx to USARTx_Rx*/
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);
  
	//adc3 channel 1	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);


}


#include "stm32f4xx.h"
#include "math.h"

DAC_InitTypeDef  DAC_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;

void GPIO_Config(void);
void DAC_Config(void);
uint16_t count = 1000;
uint32_t i=10000;

int main(void)
{
  /* DMA1 clock and GPIOA clock enable (to be used with DAC) */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	/* DAC Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

	GPIO_Config();
	DAC_Config();

	/* Enable DAC Channel1 */
  DAC_Cmd(DAC_Channel_1, ENABLE);

	
	
  while (1)
  {
		DAC-> DHR12R1 = count ;
		while(i>0)
		{i--;}	
	}
}

void GPIO_Config(void)
{
	 /* DAC channel 1 & 2 (DAC_OUT1 = PA.4)(DAC_OUT2 = PA.5) configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

}

void DAC_Config(void)
{
  
  /* DAC channel2 Configuration */
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);

}

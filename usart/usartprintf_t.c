
#include "stm32f4xx_it.h"

void USART3_IRQHandler(void)
{
uint16_t Value;
USART_ClearITPendingBit(USART3,USART_IT_RXNE);
Value=USART_ReceiveData(USART3);	
}

#include "c02.h"

void TIM2_Config (void)
{
	RCC->APB1ENR |= (1<<0); // enable clock for TIM2 
	TIM2->ARR = 0xffff-1;  	// ARR value
	TIM2->PSC = 72-1;      	// Prescalar value
	TIM2->CR1 |= (1<<0);  	// enable timer
	while (!(TIM2->SR & (1<<0)));  // UIF: Update interrupt flag..  This bit is set by hardware when the registers are updated
}

void Delay_us (uint16_t us)
{
	
	TIM2->CNT = 0;
	while (TIM2->CNT < us);
}

void Delay_ms (uint16_t ms)
{
	for (uint16_t i=0; i<ms; i++)
	{
		Delay_us (1000); // delay of 1 ms
	}
}
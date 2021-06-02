#include "pwm.h"

void pwm_init(void)
{
	
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN; //ENABLE THE TIMER4 clock
	RCC->APB1ENR |= RCC_AHB1ENR_GPIOBEN; //ENABLE THE GPIOB CLOCK
	GPIOB->MODER |= (2<<12); //PB6 ALTERNATE FUNCTION MODE
	GPIOB->OTYPER &= ~(1<<6); // BIT 6=0 OUTPUT PUSH PULL
	GPIOB->OSPEEDR |= (2<<12); //HIGH SPEED
	GPIOB->PUPDR |= (1<<12); // PULL UP
	GPIOB->AFR[0] |= (2<<24);  // Bits (27:26:25:24) = 0:0:1:0 --> AF2 for pin PB6
	
	
	//1. we need to enable the output chennal by configuring OCxE bit in CCER register
	TIM4->CCER |= ( 1 << 0); // 1 is bit position of OC1 
	
	//2. Auto-reload preload register enable in CR1.
	TIM4->CR1 |= (1 << 7); //To enable auto reload preload enable so that TIMx_ARR register is buffered
	TIM4->CR1 &= ~(1<<5) | ~(1<<6); //EDGE ALIGN
	TIM4->CR1 &= ~(1<<4);// COUNTER UP
	
	//3. Output compare preload enable
	TIM4->CCMR1 |= (1 << 3); //Preload register on TIMx_CCR1 enabled.
	TIM4->CCMR1 |= (6<<4); //SET PWM MODE 1
	//now setting the polarity of PWM output
	TIM4->CCER &=~ (1 << 1);  //setting as active high 
	
		
	/*
	
	The counter clock frequency CK_CNT is equal to fCK_PSC / (PSC[15:0] + 1) => CK_CNT= 50MHZ/(24+1) = 2MHZ
	
	pwm freq = F_CK_CNT/(ARR+1) = 2MHZ/(49+1) = 40KHZ     // ARR=45
	
	PWM Duty = CCR1 / (ARR+1)  = 25/(49+1) = 1/2 = 50%   //THIS IS FOR EDGE ALIGNED
	
	*/
	
	TIM4->PSC |=24;
	TIM4->ARR |= 49;
	TIM4->CCR1 |=25;
	TIM4->CR1 |= 1;//COUNTER ENABLE
	
	
}


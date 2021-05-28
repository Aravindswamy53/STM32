
	#include "stm32f446xx.h"
	
	#include "RccConfig.h"
	
	
	
	void gpio_config()
	{
		/************** STEPS TO FOLLOW *****************
    1. Enable GPIOA clock
    2. Set the PIN PA5 as output
    3. Configure the output mode i.e state, speed, and pull
    ************************************************/
		
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
		
		GPIOA->MODER |= (1<<10);
		
		GPIOA->OTYPER = 0;
		GPIOA->OSPEEDR = 0;
		//GPIOA->PUPDR &= ~(3<<10);
		}
	void delay(uint32_t x)
	{
		while(x--);
	}
	int main()
	{
		rcc_config_init();
		gpio_config();
		while(1)
		{
	  	GPIOA->BSRR |= (1<<5); //ST THE PIN A5
			delay(100000000);
		  GPIOA->BSRR |= ((1<<5) << 16);//RESET PIN PA5
			delay(100000000);
		}
}
	
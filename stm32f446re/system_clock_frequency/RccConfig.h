        #include "stm32f446xx.h"        
        
        #define PLL_M  4
	#define PLL_N  50
	#define PLL_P  0 //PLL_P=2
        void rcc_config_init()
	{
        /*************>>>>>>> STEPS FOLLOWED <<<<<<<<************
	
	1. ENABLE HSE and wait for the HSE to become Ready
	2. Set the POWER ENABLE CLOCK and VOLTAGE REGULATOR
	3. Configure the FLASH PREFETCH and the LATENCY Related Settings
	4. Configure the PRESCALARS HCLK, PCLK1, PCLK2
	5. Configure the MAIN PLL
	6. Enable the PLL and wait for it to become ready
	7. Select the Clock Source and wait for it to be set
	
	********************************************************/
		
		//1. ENABLE HSE and wait for the HSE to become Ready
		RCC->CR |= RCC_CR_HSEON;
		while(!(RCC->CR & RCC_CR_HSERDY));
		
		//2. Set the POWER ENABLE CLOCK and VOLTAGE REGULATOR
		RCC->APB1ENR |= RCC_APB1ENR_PWREN;
		PWR->CR |= PWR_CR_VOS;
		
		//3. Configure the FLASH PREFETCH and the LATENCY Related Settings
		FLASH->ACR = FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_5WS;
		
		//4. Configure the PRESCALARS HCLK, PCLK1, PCLK2
		//AHB PRESCALER
		RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
		// APB1 PRESCALER
		RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;
		//APB2 PRESCALER
		RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;
		
		//5. Configure the MAIN PLL
		RCC->PLLCFGR = (PLL_M <<0) | (PLL_N<<6) | (PLL_P<<16) | (RCC_PLLCFGR_PLLSRC_HSE);
		
		//6. Enable the PLL and wait for it to become ready
		RCC->CR |= RCC_CR_PLLON;
		while(!(RCC->CR & RCC_CR_PLLRDY));
		
		//7. Select the Clock Source and wait for it to be set
		RCC->CFGR |= RCC_CFGR_SW_PLL;
		while((RCC->CFGR & RCC_CFGR_SWS)!= RCC_CFGR_SWS_PLL);
         }
	  
				 
	void TIM6Config (void)
{
	/************** STEPS TO FOLLOW *****************
	1. Enable Timer clock source
	2. Set the prescalar and the ARR
	3. Enable the Timer, and wait for the update Flag to set
	************************************************/

// 1. Enable Timer clock
	RCC->APB1ENR |= (1<<4);  // Enable the timer6 clock
	
// 2. Set the prescalar and the ARR
	TIM6->PSC = 50-1;  // 50MHz/50 = 1 MHz ~~ 1 uS delay
	TIM6->ARR = 0xffff;  // MAX ARR value
	
// 3. Enable the Timer, and wait for the update Flag to set
	TIM6->CR1 |= (1<<0); // Enable the Counter
	while (!(TIM6->SR & (1<<0)));  // UIF: Update interrupt flag..  This bit is set by hardware when the registers are updated
}

void Delay_us (uint16_t us)
{
	/************** STEPS TO FOLLOW *****************
	1. RESET the Counter
	2. Wait for the Counter to reach the entered value. As each count will take 1 us, 
		 the total waiting time will be the required us delay
	************************************************/
	TIM6->CNT = 0;
	while (TIM6->CNT < us);
}

void Delay_ms (uint16_t ms)
{
	int i;
	for (i=0; i<ms; i++)
	{
		Delay_us (1000); // delay of 1 ms
	}
}


	 void setup()
	 {
						rcc_config_init();
						TIM6Config();
	 }
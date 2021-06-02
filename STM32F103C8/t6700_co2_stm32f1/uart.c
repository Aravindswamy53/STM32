#include "stm32f10x.h"

/*
USART1 TX-PA9
       RX=PA10
*/

void usart_init(void)
{
	/* ENABLE THE UART SYSTEM CLOCK AND GPIO A CLOCK */
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;//ENABLE USART1
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;//ENABLE GPIOA
	
	/*ENABLE THE ALTERNATE I/O CLOCK*/
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	
	/* CONFIGURE THE UART PINS FOR ALTERNATE FUNCTION*/
	GPIOA->CRH |= (2<<4)| (2<<6); // SET OUTPUT MODE 2MHZ [1:0] IN MODE9 AND ALTERNATE FUNCTION OUTPUT PP [1:0] IN CNF9 
	GPIOA->CRH |= (0<<8)| (1<<10); // SET INPUT MODE [0:0] IN MODE10 AND FLOATING INPUT  [0:1] IN CNF10 
	
	/*By default, the default pins like PA9 and PA10 for USART1 are used and not remapped*/
	
	/*ENABLE USART1*/
	USART1->CR1 |= USART_CR1_UE;
}

void usart_configure(uint32_t baudrate)
{
	uint32_t tmp=0;
	uint32_t integerdivider=0;
	uint32_t fractionaldivider=0;
	uint32_t fck=72;
	
	/*DEFINE WORD LENGTH M BIT IN USART1_CR1*/
	USART1->CR1 &= ~(USART_CR1_M); // WORD LENGHT 8
	
	/* BAUD RATE CALCULATION*/
	integerdivider = ((25 * fck*1000000) / (4 * baudrate));
	tmp = (integerdivider / 100) << 4;
  fractionaldivider = integerdivider - (100 * (tmp >> 4));
	tmp |= ((((fractionaldivider * 16) + 50) / 100)) & ((uint8_t)0x0F);
	
	USART1->BRR |= (uint16_t)tmp; // BAUDRATE
	
	/* TX AND RX BIT ENABLE*/
	USART1->CR1 |= USART_CR1_RE | USART_CR1_TE;	
}

void usart_send(uint8_t c)
{
	USART1->DR =c;//WRITE DATA TO USART_DR REGISTER
	while(!(USART1->SR & (1<<6)));//CHECK TC BIT FOR SUCCESSFULL TRANSMISSION
}

void usart1_sendstring(char *string)
{
	while(*string)
	{
		usart_send(*string++);
	}
}

uint8_t rec_data(void)
{
	uint8_t temp;
	while(!(USART1->SR&(1<<5)));// WAIT UNTIL RXBE BIT TO SET
	temp= USART1->DR;
	return temp;
}
 void usart_rec_string(uint16_t *string, uint32_t size)
 {
	 int i=0;
	 while(size>0)
	 {
			string[i]=rec_data();
		 ++i;
		 --size;
	 }
	 	 
 }

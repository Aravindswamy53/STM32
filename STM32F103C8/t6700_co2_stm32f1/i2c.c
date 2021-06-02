#include "c02.h"

/*
I2C1

    SDA – PB7 
    SCL – PB6 
		
*/

void i2c_init(void)
{
	/* ENABLE THE UART SYSTEM CLOCK AND GPIO A CLOCK */
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;//ENABLE USART1
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;//ENABLE GPIOA
	
	/*ENABLE THE ALTERNATE I/O CLOCK*/
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	
	/* CONFIGURE THE UART PINS FOR ALTERNATE FUNCTION*/
	GPIOA->CRL |= 0XF0000000; // SET OUTPUT MODE 50MHZ [1:1] IN MODE9 AND ALTERNATE FUNCTION OUTPUT OD [1:1] IN CNF7 
	GPIOA->CRL |= (0X3<<24)| (0X3<<26); // SET OUTPUT MODE [:1] IN MODE10 AND ALTERNATE FUNCTION OUTPUT OD  [1:1] IN CNF6
	
	/* I2C CONFIGURE*/
	//RESET I2C
	I2C1->CR1 |= (1<<15); //ENABLE SWRT
	I2C1->CR1 &= ~(1<<15); //0 IN SWRT THIS USED TO REINITIALIZE I2C
	
	/*INPUT CLOCK SPEED IN I2C */
	I2C1->CR2 |= I2C_CR2_FREQ_5;
	
	/*DUTY CYCLE*/
	I2C1->CR1 &= ~I2C_CR1_PE;	//PERIPHERAL DISABLE TO CONFIGURE CCR REGISTER
	I2C1->CCR &= ~I2C_CCR_FS;	//STANDARD MODE SCL FREQUENCY=100KHZ
	I2C1->CCR |= 160;					//TO KNOW CCR VALUE--> CCR = (1/(2*SCL FREQUENCY))/(1/FREQ IN CR2) = (1/(2*100KHZ))/(1/32KHZ)
	
	/*CONFIGURE RISE TIME REGISTER*/
	I2C1->TRISE = 33;
	
	/*ACK ENABLE*/
	I2C1->CR1 |= I2C_CR1_ACK;
	
	/* ENABLE I2C PERIPHERAL IN CR1*/
	I2C1->CR1 |= I2C_CR1_PE;	
	
}

void i2c_start_transmission(uint8_t address)
{
	uint8_t TEMP;
	I2C1->CR1 |= (1<<8);//GENERATE START BIT=1
	while(!(I2C1->SR1 & (1<<9)));//WAIT FOR SB BIT TO SET
	I2C1->DR =address;//SEND THE SLAVE ADDRESS TO DR REGISTER
	while(!(I2C1->SR1 & (1<<1)));//WAIT FOR ADDRESS BIT SET
	TEMP= I2C1->SR1 | I2C1->SR2;//CLEAR ADDRESS
}
void i2c_write(uint8_t data)
{
	while(!(I2C1->SR1 & (1<<7)));//WAIT FOR TXE TO SET. THIS INDICATE THE DR IS EMPTY
	I2C1->DR =data;//SEND DATA
	while(!(I2C1->SR1 & (1<<2)));//WAIT FOR BTF BIT TO SET
}

void i2c_read(uint8_t address,uint16_t *buffer,uint32_t size)
{
	uint8_t TEMP;
	int remaining =size;
	if(size ==1)
	{
		I2C1->DR = address;//SEND ADDRESS
		while(!(I2C1->SR1 &(1<<1)));//WAIT FOR ADDRESS BIT TO SET
		I2C1->CR1 &= ~(1<<10);//CLEAR ACK
		TEMP= I2C1->SR1 | I2C1->SR2;//CLEAR ADDRESS
		I2C1->CR1 |= (1<<9);//STOP I2C
		while(!(I2C1->SR1 &(1<<6)));//WAIT FOR RXNE
		buffer[size-remaining]=I2C1->DR;
	}
	else
	{
		I2C1->DR = address;
		while(!(I2C1->SR1 &(1<<1)));
		TEMP= I2C1->SR1 | I2C1->SR2;
		while(remaining>2)
		{
			while(!(I2C1->SR1 &(1<<6)));
			buffer[size-remaining]=I2C1->DR;
			I2C1->CR1 &= ~(1<<10);
			remaining--;
		}
		while(!(I2C1->SR1 &(1<<6)));
		buffer[size-remaining]=I2C1->DR;
		I2C1->CR1 &= ~(1<<10);
		I2C1->CR1 &= ~(1<<9);
		remaining--;
		while(!(I2C1->SR1 &(1<<6)));
		buffer[size-remaining]=I2C1->DR;
	}
}

void i2c_stop(void)
{
	I2C1->CR1 |= (1<<9);  //STOP I2C
}


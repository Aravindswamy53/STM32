#include "c02.h"
uint16_t data_i2c_co2[10];
uint16_t data_uart_co2[10];
uint32_t co2_i2c_value;
uint32_t co2_uart_value;
int main()
{
	char string_uart[9]={0x15,0x4,0x13,0x89,0x00,0x01};        // TRANSMIT DATA (USART)
	unsigned char data_crc[2]={0x00,0x01};// msb lsb        
	uint16_t crc=0;
	
	SystemInit ();
	TIM2_Config();
	
	usart_init();                                              //USART INITIALIZATION
	usart_configure(19200);                                    //UASRT CONFIGURE TO 19200 BAUD RATE
	crc = CRC16(data_crc,2);                           //CRC VALUE
	string_uart[6]=(crc>>8);                                   
	string_uart[7]=(crc & 0x00FF);                             
	string_uart[8]='\0';  
	
	i2c_init();                                                //I2C INITIALIZATION
	
	                                     
	while(1) 
	{
		/* i2c */
		i2c_start_transmission(0x15);
		i2c_write(0x4);
		i2c_write(0x13);
		i2c_write(0x89);
		i2c_write(0x00);
		i2c_write(0x01);
		i2c_stop();
		Delay_us(2000);
		i2c_read(0x15,data_i2c_co2,4);
		co2_i2c_value = ((data_i2c_co2[2] <<8) | data_i2c_co2[3]);
		Delay_us(2000);
		
/* uart 
	• 19200 Baud
	• 1 START bit
	• 8 DATA bits
	• 1 EVEN PARITY bit
	• 1 STOP bit  
*/
		usart1_sendstring(string_uart);
		Delay_us(2000);
		usart_rec_string(data_uart_co2, 7);
		
		co2_uart_value=((data_uart_co2[3]<<8) | data_uart_co2[4]);
		Delay_us(2000);
		
	}
}



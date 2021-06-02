#include "stm32f10x.h"
#include "stdio.h"

void TIM2_Config (void);
void Delay_us (uint16_t us);
void Delay_ms (uint16_t ms);

void usart_init(void);
void usart_configure(uint32_t baudrate);
void usart_send(uint8_t c);
void usart1_sendstring(char *string);
uint8_t rec_data(void);
void usart_rec_string(uint16_t *string,uint32_t size);

void i2c_init(void);
void i2c_start_transmission(uint8_t address);
void i2c_write(uint8_t data);
void i2c_read(uint8_t address,uint16_t *buffer,uint32_t size);
void i2c_stop(void);


unsigned short CRC16 (unsigned char *puchMsg,unsigned short usDataLen );

#include "i2c_avr.h"
#include <avr/io.h>
#include <util/delay.h>

void I2CInit(uint16_t CPU_F,  uint32_t SCL_F)
{
	//Set up TWI Module
	TWBR = ( ( (CPU_F)/(SCL_F)-16 ) /2 ) ;
    TWSR = 0; //


	//Enable the TWI Module
	TWCR|=(1<<TWEN);
}

void I2CClose()
{
	//Disable the module
	TWCR&=(~(1<<TWEN));
}


void I2CStart()
{
	//Put Start Condition on Bus
	TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWSTA);

	//Poll Till Done
	while(!(TWCR & (1<<TWINT)));

}

void I2CStop()
{
	//Put Stop Condition on bus
	TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	
	//Wait for STOP to finish
	while(TWCR & (1<<TWSTO));
	//_delay_loop_2(250);
}

uint8_t I2CWriteByte(uint8_t data)
{
	
	TWDR=data;	

	//Initiate Transfer
	TWCR=(1<<TWEN)|(1<<TWINT);

	//Poll Till Done
	while(!(TWCR & (1<<TWINT)));

	//Check Status
	if((TWSR & 0xF8) == 0x18 || (TWSR & 0xF8) == 0x28 || (TWSR & 0xF8) == 0x40)
	{
		//SLA+W Transmitted and ACK received
		//or
		//SLA+R Transmitted and ACK received
		//or
		//DATA Transmitted and ACK recived

		return TRUE;
	}
	else
		return FALSE;	//Error
}

uint8_t I2CReadByte(uint8_t *data,uint8_t ack)
{
	//Set up ACK
	if(ack)
	{
		//return ACK after reception
		TWCR|=(1<<TWEA);
	}
	else
	{
		//return NACK after reception
		//Signals slave to stop giving more data
		//usually used for last byte read.
		TWCR&=(~(1<<TWEA));
	}

	//Now enable Reception of data by clearing TWINT
	TWCR|=(1<<TWINT);

	//Wait till done
	while(!(TWCR & (1<<TWINT)));

	//Check status
	if((TWSR & 0xF8) == 0x58 || (TWSR & 0xF8) == 0x50)
	{
		//Data received and ACK returned
		//	or
		//Data received and NACK returned

		//Read the data

		*data=TWDR;
		return TRUE;
	}
	else
		return FALSE;	//Error
	
}

void I2CWriteRegister(uint8_t addr, uint8_t reg, uint8_t value)

{
   I2CStart();
   I2CWriteByte((addr<<1));
   I2CWriteByte(reg);
   I2CWriteByte(value);
   _delay_ms(10);
   I2CStop();
   _delay_ms(6);
}

void I2CReadBytes(uint8_t dev_addr, uint8_t register_addr, uint8_t num, uint8_t *buffer)
{
	I2CStart();
	I2CWriteByte(dev_addr<<1);
	I2CWriteByte(register_addr);  //Accelerometer Measurements

	I2CStart();
	I2CWriteByte((dev_addr<<1)+1);

	for (uint8_t i = 0; i < num; i++) {
		I2CReadByte(buffer,1);
	    buffer++;
	}
}

/*int32_t I2CgetRegister(uint8_t dev_addr, uint8_t register_addr, uint8_t num, uint8_t unsign) {
	uint8_t buffer[4];
    int32_t result32 = 0;
    int16_t result16 = 0;
    int8_t result8   = 0;
    
    I2CReadBytes(dev_addr, register_addr, num, buffer);

    for (int i = 0; i < num; i++) {
        result32 <<= 8;
        result32 += buffer[i];
    }

    //	добавляем знак (если Uns==true)
    if (unsign==1 && num == 1) {
        result8  = result32;
        result32 = result8;
    }

    //	добавляем знак (если Uns==true)
    if (unsign==1 && num == 2) {
        result16 = result32;
        result32 = result16;
    }
    return result32;
}
*/

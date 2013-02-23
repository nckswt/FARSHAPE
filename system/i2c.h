#ifndef _ENCODER_H
#define _ENCODER_H

#define I2C_BUS 1
#include <stdint.h>
#include <unistd.h>

void i2c_write( int chipAddress , int dataAddress , int* data, int length = 1, int bus = I2C_BUS );
void i2c_write( int chipAddress , int dataAddress , int data, int bus = I2C_BUS);

uint8_t i2c_read( int chipAddress , int dataAddress , int length = 1, int bus = I2C_BUS );	
int open_chip( int bus, int address );
int open_i2c_dev(int i2cbus, char *filename, size_t size, int quiet);

#endif
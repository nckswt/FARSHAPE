#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
// #include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include "i2cbusses.h"
#include "i2c.h"
// #include <i2c/smbus.h>

void i2c_write( int chipAddress , int dataAddress , int* data, int length, int bus ) {
  // file handle
  int i2c_file;
  // open communications with a specific chip
  i2c_file = open_chip( bus, chipAddress );
  /* TODO: add error handling */
  for (int i=0; i<length; i++) {
    i2c_smbus_write_byte_data(i2c_file, dataAddress, *(data+i));
  }

}

void i2c_write( int chipAddress , int dataAddress , int data, int bus ) {
  
  // file handle
  int i2c_file;
  
  // open communications with a specific chip
  i2c_file = open_chip( bus, chipAddress );
  /* TODO: add error handling */
  
  // write the data to a specific address on the abovementioned chip
  i2c_smbus_write_byte_data(i2c_file, dataAddress, data);
  
}

uint8_t i2c_read( int chipAddress, int dataAddress, int length, int bus ) {
  
  // file handle
  int i2c_file;
  
  // open communications with a specific chip
  i2c_file = open_chip( bus, chipAddress );
  /* TODO: add error handling */
  
  // read the specified number of bytes
  for (int i=0; i < length; i++) {
    i2c_smbus_read_byte_data(i2c_file, dataAddress+i);
  }
  
}

int open_chip(int bus, int address) {
  char *end;
  int file;
  char filename[20];
  file = open_i2c_dev(bus, filename, sizeof(filename), 0);
  if (file < 0 || set_slave_addr(file, address, 0))
	  exit(1);
  return file;
}

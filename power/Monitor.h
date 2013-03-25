#include "i2c.h"
#include "i2cbusses.h"

#define CONFIGURATION_REGISTER 0x00
#define SHUNT_VOLTAGE_REGISTER 0x01
#define BUS_VOLTAGE_REGISTER 0x02
#define POWER_REGISTER 0x03
#define CURRENT_REGISTER 0x04
#define CALIBRATION_REGISTER 0x05

#define 320MV 0b11
#define 160MV 0b10
#define 80MV 0b01
#define 40MV 0b00

class Monitor {
private:
  int chipAddress;
  int bus;
  int voltageScaling;
  void _assert( int flag );
  void _write( int location, int data );
  int _read( int location );
  signed int _readVoltage();
  signed int _readCurrent(); 
  unsigned int _readPower();
  void setVoltageScaling();
  

public:
  Monitor( int address, int voltageScaling, int bus = I2C_BUS );
  ~Monitor();
  signed int getCurrent();
  signed int getVoltage();
};
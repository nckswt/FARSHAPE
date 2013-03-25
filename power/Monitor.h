#include "../system/i2c.h"
#include "../system/i2cbusses.h"

#define CONFIGURATION_REGISTER 0x00
#define SHUNT_VOLTAGE_REGISTER 0x01
#define BUS_VOLTAGE_REGISTER 0x02
#define POWER_REGISTER 0x03
#define CURRENT_REGISTER 0x04
#define CALIBRATION_REGISTER 0x05

#define mv_320 0b11
#define mv_160 0b10
#define mv_80 0b01
#define mv_40 0b00

#define V5_BUS 0x40
#define V9_BUS 0x45
#define V11_BUS 0x44

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
  void setVoltageScaling( );
  

public:
  Monitor( int address, int voltageScaling, int bus = I2C_BUS );
  ~Monitor();
  signed int getCurrent();
  signed int getVoltage();
};
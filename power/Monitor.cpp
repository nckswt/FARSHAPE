Monitor::Monitor( int address, int voltageScaling, int bus ) {
  
  this->chipAddress = address; 
  this->bus = bus;
  this->voltageScaling = voltageScaling;
  _write( CALIBRATION_REGISTER, lsb );
  setVoltageScaling( voltageScaling );

}

signed int Monitor::_readVoltage() {
  // Bus voltage register is not right-aligned, so shift right by 3
  return this->_read( BUS_VOLTAGE_REGISTER ) >> 3;
}
signed int Monitor::_readCurrent() {
  // Read current register
  return this->_read( CURRENT_REGISTER );
}

unsigned int Monitor::_readPower() {
  // read power register
  return this->_read( POWER_REGISTER );
}

void Monitor::setVoltageScaling() {
  
  int current_setting = _read( CONFIGURATION_REGISTER );
  _write( CONFIGURATION_REGISTER, (this->voltageScaling << 11) | current_setting );
}

void Monitor::_assert(int flag) {
  i2c_write_probe( this->chipAddress , flag );
}

void Monitor::_write( int location, int data ) {
  i2c_write_word( this->chipAddress, location, data );
}

int Monitor::_read( int location ) {
  return i2c_read_word ( this->chipAddress, location );
}

signed int Monitor::getCurrent() {
  return _readCurrent();
}

signed int Monitor::getVoltage() {
  return _readVoltage();
}
  
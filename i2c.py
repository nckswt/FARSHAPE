import smbus

#defines
ENCODER_1_ADDRESS = 0x20
ENCODER_2_ADDRESS = 0x21
ADC_ADDRESS = 0x00

bus = smbus.SMBus(0)

def setup_encoders():
  
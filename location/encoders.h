#define ENCODER1_ADDRESS 0x20
#define ENCODER2_ADDRESS 0x22
#define ENCODER_DEFAULT_ADDRESS 0x30

#define DISABLE_TERMINATOR 0x4B
#define ENABLE_TERMINATOR 0x4C
#define CHANGE_DEVICE_ADDRESS 0x4D

// maybe these should be declared in an array / iterator? 
#define ROTATION_0 0x41
#define ROTATION_1 0x40
#define ROTATION_2 0x43
#define ROTATION_3 0x42
#define ROTATION_4 0x47
#define ROTATION_5 0x46
#define ROTATION_BYTES 6

#define SIGNED_VELOCITY_0 0x3E
#define SIGNED_VELOCITY_1 0x3F
#define SIGNED_VELOCITY_BYTES 2

class Encoder {
private:
  uint64_t getEncoderPosition();
  uint8_t chipAddress;
  bool finalEncoder;
public:
  Encoder( int chipAddress , bool finalEncoder, int bus = 1 );
  ~Encoder( );
  float getEncoderDistance();
};
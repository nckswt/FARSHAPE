CC      := g++
CFLAGS  ?= -O2
CFLAGS  += -Wall
CXXFLAGS = -O2 -Wall

I2C_OBJECTS=system/i2c.o system/i2cbusses.o
MOTORS_OBJ=mobility/Motors.o

tests: encoder_test

ENCODER_TEST_OBJS=$(I2C_OBJECTS) location/Encoder.o
encoder_test: tests/encoder_test.c $(ENCODER_TEST_OBJS)
	$(CC) tests/encoder_test.c $(ENCODER_TEST_OBJS) -o tests/encoder_test.exe

DRIVE_DISTANCE_TEST_OBJS=$(I2C_OBJECTS) location/Encoder.o location/Location.o system/Behaviours.o
drive_distance_test: tests/drive_distance_test.cpp $(DRIVE_DISTANCE_TEST_OBJS)
	$(CC) tests/drive_distance_test.cpp $(DRIVE_DISTANCE_TEST_OBJS) -o tests/drive_distance_test.exe

FUCKYOU_TEST_OBJS=$(I2C_OBJECTS) location/Encoder.o $(MOTORS_OBJ)
fuckyou_test: tests/fuckyou.cpp $(FUCKYOU_TEST_OBJS)
	$(CC) tests/fuckyou.cpp -lpthread $(FUCKYOU_TEST_OBJS) -o tests/fuckyou.exe

DRIVER_TEST_OBJS=$(ENCODER_TEST_OBJS) system/Pilot.o system/Navigator.o mobility/Motor.o
driver_test: tests/driver_test.cpp $(DRIVER_TEST_OBJS) 
	$(CC) tests/driver_test.cpp -lpthread $(DRIVER_TEST_OBJS) -o tests/driver_test.exe

NAV_TEST_OBJS=$(ENCODER_TEST_OBJS) system/Navigator.o mobility/Motor.o
nav_test: tests/nav_test.cpp $(NAV_TEST_OBJS)
	$(CC) tests/nav_test.cpp $(NAV_TEST_OBJS) -o tests/nav_test.exe
clean:
	rm ./*/*.o
	rm ./*/*.exe

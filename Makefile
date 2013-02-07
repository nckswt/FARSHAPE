CC      := g++
CFLAGS  ?= -O2
CFLAGS  += -Wall
CXXFLAGS= -O2 -Wall
SOURCES=location/Encoder.c system/i2c.c system/i2cbusses.c
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLES=tests/encoder_test.c
I2C_OBJECTS=system/i2c.o system/i2cbusses.o
MOTORS_OBJ=mobility/Motors.o
ENCODER_TEST_OBJS=

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

clean:
	rm ./*/*.o
	rm ./*/*.exe
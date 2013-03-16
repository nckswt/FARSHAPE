CC      := g++
CFLAGS  ?= -O2
CFLAGS  += -Wall
CXXFLAGS = -O2 -Wall
# CXXFLAGS+= -I/opt/ros/fuerte/include/
I2C_OBJECTS=system/i2c.o system/i2cbusses.o

tests: encoder_test motor_test

ENCODER_TEST_OBJS=$(I2C_OBJECTS) location/Encoder.o
encoder_test: tests/encoder_test.c $(ENCODER_TEST_OBJS)
        $(CXX) tests/encoder_test.c $(ENCODER_TEST_OBJS) -o tests/encoder_test.exe

MOTOR_TEST_OBJS=mobility/Motor.o
motor_test: tests/motor_test.cpp $(MOTOR_TEST_OBJS)
        $(CC) tests/motor_test.cpp $(MOTOR_TEST_OBJS) -o tests/motor_test.exe

IR_TEST_OBJS=location/IRsensor.o
ir_test: tests/ir_test.cpp $(IR_TEST_OBJS)
        $(CC) tests/ir_test.cpp -lwiringPi $(IR_TEST_OBJS) -o tests/ir_test.exe

DRIVER_TEST_OBJS=$(ENCODER_TEST_OBJS) system/Pilot.o system/Navigator.o mobility/Motor.o
driver_test: tests/driver_test.cpp $(DRIVER_TEST_OBJS)
        $(CC) tests/driver_test.cpp -lpthread $(DRIVER_TEST_OBJS) -o tests/driver_test.exe

NAV_TEST_OBJS=$(ENCODER_TEST_OBJS) system/Navigator.o mobility/Motor.o
nav_test: tests/nav_test.cpp $(NAV_TEST_OBJS)
        $(CC) tests/nav_test.cpp $(NAV_TEST_OBJS) -o tests/nav_test.exe

HELM_TEST_OBJS=$(MOTOR_TEST_OBJS) $(ENCODER_TEST_OBJS) system/Helm.o
helm_test: tests/helm_test.cpp $(HELM_TEST_OBJS)
        $(CXX) tests/helm_test.cpp $(HELM_TEST_OBJS) -o tests/helm_test.exe

read_test: tests/read_test.cpp $(HELM_TEST_OBJS)
        $(CXX) tests/read_test.cpp $(HELM_TEST_OBJS) -o tests/read_test.exe

clean:
        rm ./*/*.o
        rm ./*/*.exe


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
	$(CXX) tests/motor_test.cpp $(MOTOR_TEST_OBJS) -o tests/motor_test.exe

HELM_TEST_OBJS=$(MOTOR_TEST_OBJS) $(ENCODER_TEST_OBJS) system/Helm.o
helm_test: tests/helm_test.cpp $(HELM_TEST_OBJS)
	$(CXX) tests/helm_test.cpp $(HELM_TEST_OBJS) -o tests/helm_test.exe

IR_TEST_OBJS=location/IRsensor.o
ir_test: tests/irtest.cpp $(IR_TEST_OBJS)
	$(CXX) tests/irtest.cpp $(IR_TEST_OBJS) -o tests/irtest.exe

read_test: tests/read_test.cpp $(HELM_TEST_OBJS)
	$(CXX) tests/read_test.cpp $(HELM_TEST_OBJS) -o tests/read_test.exe

clean:
	rm ./*/*.o
	rm ./*/*.exe

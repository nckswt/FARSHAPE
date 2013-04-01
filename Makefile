CC      := g++
CFLAGS  ?= -O2
CFLAGS  += -Wall
CXXFLAGS = -O2 -Wall
CXXFLAGS+= -I/opt/ros/fuerte/include/ 
LDFLAGS=-lpthread
I2C_OBJECTS=system/i2c.o system/i2cbusses.o
ROS_LINKS=-L/opt/ros/fuerte/lib -lroscpp -lrostime -lrosconsole -lroscpp_serialization -lxmlrpcpp
WIRING_PI_LINKS=-lwiringPi -lpthread

tests: encoder_test motor_test 

ENCODER_TEST_OBJS=$(I2C_OBJECTS) location/Encoder.o
encoder_test: tests/encoder_test.c $(ENCODER_TEST_OBJS)
	$(CC) tests/encoder_test.c $(ENCODER_TEST_OBJS) -o tests/encoder_test.exe

MOTOR_TEST_OBJS=mobility/Motor.o
motor_test: tests/motor_test.cpp $(MOTOR_TEST_OBJS)
	$(CC) tests/motor_test.cpp $(MOTOR_TEST_OBJS) -o tests/motor_test.exe

HELM_TEST_OBJS=$(MOTOR_TEST_OBJS) $(ENCODER_TEST_OBJS) system/Helm.o
helm_test: tests/helm_test.cpp $(HELM_TEST_OBJS)
	$(CC) tests/helm_test.cpp $(HELM_TEST_OBJS) $(ROS_LINKS) -o tests/helm_test.exe

read_test: tests/read_test.cpp $(HELM_TEST_OBJS)
	$(CXX) -I/opt/ros/fuerte/include/ tests/read_test.cpp $(HELM_TEST_OBJS) $(ROS_LINKS) -o tests/read_test.exe

DRIVER_OBJS=$(HELM_TEST_OBJS) sensing/camera.o 
driver: tests/driver.cpp $(DRIVER_OBJS)
	$(CXX) tests/driver.cpp $(DRIVER_OBJS) -o driver.exe

MONITOR_OBJS=power/Monitor.o $(I2C_OBJECTS)
monitor_test: tests/monitor_test.cpp $(MONITOR_OBJS)
	$(CXX) tests/monitor_test.cpp $(MONITOR_OBJS) -o tests/monitor_test.exe

CONFIG_IR_OBJS=location/IRsensor.o system/ADC_SPI.o
config_ir: tests/config_ir.cpp $(CONFIG_IR_OBJS)
	$(CXX) tests/config_ir.cpp $(CONFIG_IR_OBJS) $(WIRING_PI_LINKS) -o tests/config_ir.exe

IR_TEST_OBJS=location/IRsensor.o system/ADC_SPI.o
ir_test: tests/irtest.cpp $(IR_TEST_OBJS)
	$(CXX) tests/irtest.cpp $(IR_TEST_OBJS) $(WIRING_PI_LINKS) -o tests/irtest.exe

ADC_TEST_OBJS=system/ADC_SPI.o
adc_test: tests/adc_test.cpp $(ADC_TEST_OBJS)
	$(CXX) tests/adc_test.cpp $(ADC_TEST_OBJS) $(WIRING_PI_LINKS) -o tests/adc_test.exe

clean:
	rm ./*/*.o
	rm ./*/*.exe
CC      := g++
CFLAGS  ?= -O2
CFLAGS  += -Wall
SOURCES=location/Encoder.c system/i2c.c system/i2cbusses.c
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLES=tests/encoder_test.c
I2C_OBJECTS=system/i2c.o system/i2cbusses.o
ENCODER_TEST_OBJS=

tests: encoder_test

ENCODER_TEST_OBJS=$(I2C_OBJECTS) location/Encoder.o
encoder_test: tests/encoder_test.c $(ENCODER_TEST_OBJS)
	$(CC) tests/encoder_test.c $(ENCODER_TEST_OBJS) -o tests/encoder_test.exe

clean:
	rm ./*/*.o
	rm ./*/*.exe
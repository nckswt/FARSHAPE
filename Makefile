CC      := g++
CFLAGS  ?= -O2
CFLAGS  += -Wall

all:

encoder_test:
	g++ system/i2c.c system/i2cbusses.c tests/encoder_test.c location/encoders.c

#encoders: i2c
#	g++ location/encoders.c
#
#i2c: system/i2c.c system/i2cbusses.c
#	g++ system/i2c.c system/i2cbusses.c


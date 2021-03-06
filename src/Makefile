################
#
#  Makefile for
#  I2c stuff
#  Author: Justin M. Bradley
#
################

OETMP = /home/gumstix/overo-oe/tmp
STAGEDIR = $(OETMP)/sysroots/armv7a-angstrom-linux-gnueabi/usr
GUMLIB = $(STAGEDIR)/lib
GUMINC = $(STAGEDIR)/include

CC = gcc # Compile for Linux
CXX = g++ # Compile for Linux
GUMCC = $(OETMP)/sysroots/i686-linux/usr/armv7a/bin/arm-angstrom-linux-gnueabi-gcc
GUMCXX = $(OETMP)/sysroots/i686-linux/usr/armv7a/bin/arm-angstrom-linux-gnueabi-g++
CFLAGS = -W -O3 -Wall -Wextra -ansi -pedantic
GUMFLAGS = $(FLAGS) -I $(OETMP)/sysroots/overo-angstrom-linux-gnueabi/kernel/include/linux/i2c/ -L $(GUMLIB) -I $(GUMINC)
LD_FLAGS = -lrt -lm
INCLUDE = -I../include
OPENCV_FLAGS =  `pkg-config --cflags --libs opencv`

O_FILES = Control.o Camera.o Utilities.o tsat_main.o
EXECUTABLE = ../tsat571

I2C_O_FILES = i2c-api.o I2c_comm.o test_i2c.o
I2C_EXEC = ../i2c

SOCK_EXEC = ../sock
ADC_EXEC = ../adc
PWM_EXEC = ../pwm
COMM_EXEC = ../test_Comm
LOOP_EXEC = ../loop
VELS_EXEC = ../test_vels

# Default make target is first
#
all:	$(O_FILES)
	$(GUMCXX) $(OPENCV_FLAGS) $(CFLAGS) $(GUMFLAGS) -o $(EXECUTABLE) $(O_FILES) $(INCLUDE) $(LD_FLAGS)
	chmod 711 $(EXECUTABLE)

i2c:	$(I2C_O_FILES)
	$(GUMCXX) $(CFLAGS) $(GUMFLAGS) -o $(I2C_EXEC) $(I2C_O_FILES) $(INCLUDE) $(LD_FLAGS)
	chmod 711 $(I2C_EXEC)

test_adc:	adc.o test_adc.c
		$(GUMCXX) $(CFLAGS) $(GUMFLAGS) -o $(ADC_EXEC) test_adc.c adc.o $(INCLUDE) $(LD_FLAGS)
		chmod 711 $(ADC_EXEC)

test_pwm:	test_pwm.c ../include/pwm.h
		$(GUMCXX) $(CFLAGS) $(GUMFLAGS) -o $(PWM_EXEC) test_pwm.c $(INCLUDE) $(LD_FLAGS)

test_server:	test_server.c simple_sock.c ../include/simple_sock.h
		$(GUMCXX) $(CFLAGS) $(GUMFLAGS) -o $(SOCK_EXEC) test_server.c simple_sock.c $(INCLUDE) $(LD_FLAGS)
		chmod 711 $(SOCK_EXEC)

test_Comm:	./test_Comm.c ./Comm.c ../include/simple_sock.h ../include/Comm.h 
		$(CXX) $(CFLAGS) -o $(COMM_EXEC) test_Comm.c Comm.c simple_sock.c $(INCLUDE)
		chmod 711 $(COMM_EXEC)

test_vels:	./Utilities.o ./test_vels.c ../include/pwm.h
		$(GUMCXX) $(CFLAGS) $(GUMFLAGS) -o $(VELS_EXEC) test_vels.c Utilities.o $(INCLUDE) $(LD_FLAGS)

task_loop:	./task_loop.c ../include/pwm.h
		$(GUMCXX) $(CFLAGS) $(GUMFLAGS) -o $(LOOP_EXEC) task_loop.c $(INCLUDE) $(LD_FLAGS)

test_i2c.o:	test_i2c.c
		$(GUMCXX) $(CFLAGS) $(GUMFLAGS) -o $@ -c $< $(INCLUDE)

tsat_main.o:	tsat_main.c
		$(GUMCXX) $(CFLAGS) $(GUMFLAGS) -o $@ -c $< $(INCLUDE)

%.o:	%.c ../include/%.h
	$(GUMCXX) $(OPENCV_FLAGS) $(CFLAGS) $(GUMFLAGS) -o $@ -c $< $(INCLUDE)

clean:
	rm -rf *.o $(EXECUTABLE) $(I2C_EXEC) $(SOCK_EXEC) $(ADC_EXEC) $(PWM_EXEC) $(COMM_EXEC) $(LOOP_EXEC) $(VELS_EXEC)

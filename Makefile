################
#
#  Makefile for
#  I2c stuff
#  Author: Justin M. Bradley
#
################

all:
	(cd src/; make;)

i2c:	
	(cd src/; make i2c;)

test_adc:
	(cd src/; make test_adc;)

test_pwm:
	(cd src/; make test_pwm;)

test_server:	
	(cd src/; make test_server;)

test_Comm:
	(cd src/; make test_Comm;)

test_vels:
	(cd src/; make test_vels;)

send: 
	scp hc1 root@192.168.0.108:

clean:	
	(cd src/; make clean;)
###############################################################
#
# Purpose: Makefile for "dxl_monitor"
# Author.: robotis
# Version: 0.1
# License: GPL
#
###############################################################

TARGET = TwoWheeler

INCLUDE_DIRS = -I../../include -I../../../Framework/include -I./

CXX = g++
CXXFLAGS += -g -O2 -shared -D_GNU_SOURCE  -DLINUX -Wall $(INCLUDE_DIRS)
#CXXFLAGS += -O2 -DDEBUG -DLINUX -Wall $(INCLUDE_DIRS)
LFLAGS += -g -lpthread -lrt -lbluetooth -l wiringPi -l wiringPiDev -l pthread

OBJECTS = main.o

all: $(TARGET)

clean:
	rm -f *.a *.o $(TARGET) core *~ *.so *.lo

libclean:
	make -C ../../build clean

distclean: clean libclean

darwin.a:
	make -C ../../build

$(TARGET): darwin.a $(OBJECTS)
	$(CXX) $(CFLAGS) $(OBJECTS) ../../lib/darwin.a lidarLite.c  -o $(TARGET) $(LFLAGS)
	chmod 755 $(TARGET)

# useful to make a backup "make tgz"
tgz: clean
	mkdir -p backups
	tar czvf ./backups/DARwIn_demo_`date +"%Y_%m_%d_%H.%M.%S"`.tgz --exclude backups *


#gcc -o lidarTest lidarTest.c lidarLite.c -l wiringPi -l wiringPiDev -l pthread -I ../include/
 

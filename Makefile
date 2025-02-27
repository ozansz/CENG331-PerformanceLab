CC = gcc
CFLAGS = -Wall -O2 -m32
LIBS = -lm

OBJS = driver.o kernels.o fcyc.o clock.o

all: driver

driver: $(OBJS) fcyc.h clock.h defs.h config.h
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o driver

clean: 
	-rm -f $(OBJS) driver core *~ *.o




mca: $(OBJS) fcyc.h clock.h defs.h config.h
	gcc kernels.c -lm -Wall -O2 -m32 -S -o - | llvm-mca-11 -bottleneck-analysis -o analysis.txt
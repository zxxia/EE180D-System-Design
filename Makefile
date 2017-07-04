CC=gcc
LDFLAGS=-lmraa -lm -lfann
CFLAGS=-Wall -Werror -g
SRC=src
PCLASSES=${SRC}/LSM9DS0.o
CCLASSES=$(PCLASSES) ${SRC}/math_func.o ${SRC}/stride_detector.o ${SRC}/feature_detector.o ${SRC}/exe_neural_network.o

all: producer consumer

producer: $(PCLASSES)
	$(CC) $(CFLAGS) -o $@ $^ ${SRC}/$@.c -lmraa

consumer: $(CCLASSES)
	$(CC) $(CFLAGS) -o $@ $^ ${SRC}/$@.c $(LDFLAGS)


clean: 
	rm -f producer consumer
	rm -f *~
	rm -f ${SRC}/*~ ${SRC}/*.o
	rm -f .*.un~

CC=gcc
LDFLAGS=-lmraa -lfann
CFLAGS=-Wall -g
SRC=src
SOURCES=${SRC}/math_func.c ${SRC}/stride_detector.c ${SRC}/feature_detector.c ${SRC}/producer.c ${SRC}/consumer.c ${SRC}/exe_neural_network.c
EXECUTABLES=$(SOURCES:.c=)

all: producer consumer

producer: ${SRC}/producer.c ${SRC}/LSM9DS0.c
	$(CC) $(CFLAGS) -o producer ${SRC}/producer.c ${SRC}/LSM9DS0.c $(LDFLAGS)

consumer: ${SRC}/consumer.c ${SRC}/stride_detector.c ${SRC}/feature_detector.c ${SRC}/math_func.c ${SRC}/exe_neural_network.c
	$(CC) $(CFLAGS) -o consumer ${SRC}/consumer.c ${SRC}/feature_detector.c ${SRC}/stride_detector.c ${SRC}/math_func.c ${SRC}/exe_neural_network.c -lm $(LDFLAGS)


clean: 
	rm -f producer consumer
	rm -f *~
	rm -f src/*~

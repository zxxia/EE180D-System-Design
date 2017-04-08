CC=gcc
LDFLAGS=-lmraa -lfann
CFLAGS=-Wall -Werror
SRC=src
SOURCES=${SRC}/test_neural_network.c ${SRC}/train_neural_net.c ${SRC}/imu_data.c ${SRC}/stride_detector.c ${SRC}/feature_detector.c
EXECUTABLES=$(SOURCES:.c=)

all: test_neural_network train_neural_net imu_data train_file_generator stride_detector feature_detector

test_neural_network: ${SRC}/test_neural_network.c
	$(CC) $(CFLAGS) -o test_neural_network ${SRC}/test_neural_network.c $(LDFLAGS)

train_neural_net: ${SRC}/train_neural_net.c
	$(CC) $(CFLAGS) -o train_neural_net ${SRC}/train_neural_net.c $(LDFLAGS)

imu_data: ${SRC}/imu_data.c ${SRC}/LSM9DS0.c
	$(CC) $(CFLAGS) -o imu_data ${SRC}/imu_data.c ${SRC}/LSM9DS0.c $(LDFLAGS)

train_file_generator: ${SRC}/train_file_generator.c
	$(CC) $(CFLAGS) -o train_file_generator ${SRC}/train_file_generator.c $(LDFLAGS)

stride_detector: ${SRC}/stride_detector.c
	$(CC) $(CFLAGS) -o stride_detector ${SRC}/stride_detector.c -lm

feature_detector: ${SRC}/feature_detector.c
	$(CC) $(CFLAGS) -o feature_detector ${SRC}/feature_detector.c -lm

clean: 
	rm -f test_neural_network train_neural_net imu_data train_file_generator stride_detector feature_detector
	rm -f *~
	rm -f TEST.net
	rm -f test_data.txt
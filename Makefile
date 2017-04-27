CC=gcc
LDFLAGS=-lmraa -lfann
CFLAGS=-Wall -Werror
SRC=src
SOURCES=${SRC}/math_func.c ${SRC}/main.c ${SRC}/test_neural_network.c ${SRC}/train_neural_net.c ${SRC}/imu_data.c ${SRC}/stride_detector.c ${SRC}/feature_detector.c
EXECUTABLES=$(SOURCES:.c=)

all: main test_neural_network train_neural_net imu_data train_file_generator

test_neural_network: ${SRC}/test_neural_network.c
	$(CC) $(CFLAGS) -o test_neural_network ${SRC}/test_neural_network.c $(LDFLAGS)

train_neural_net: ${SRC}/train_neural_net.c
	$(CC) $(CFLAGS) -o train_neural_net ${SRC}/train_neural_net.c $(LDFLAGS)

imu_data: ${SRC}/imu_data.c ${SRC}/LSM9DS0.c
	$(CC) $(CFLAGS) -o imu_data ${SRC}/imu_data.c ${SRC}/LSM9DS0.c $(LDFLAGS)

train_file_generator: ${SRC}/train_file_generator.c 
	$(CC) $(CFLAGS) -o train_file_generator ${SRC}/train_file_generator.c $(LDFLAGS)

main: ${SRC}/main.c ${SRC}/stride_detector.c ${SRC}/feature_detector.c ${SRC}/math_func.c
	$(CC) $(CFLAGS) -o main ${SRC}/main.c ${SRC}/feature_detector.c ${SRC}/stride_detector.c ${SRC}/math_func.c -lm $(LDFLAGS)

walk_feature_detector: ${SRC}/walk_feature_detector.c
	$(CC) $(CFLAGS) -o walk_feature_detector ${SRC}/walk_feature_detector.c -lm

clean: 
	rm -f main test_neural_network train_neural_net imu_data train_file_generator walk_feature_detector
	rm -f *~
	rm -f src/*~
	rm -f TEST.net
	rm -f test_data.txt

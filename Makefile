CC=gcc
LDFLAGS=-lmraa -lfann
CFLAGS=-Wall -Werror
SRC=src
SOURCES=${SRC}/test_neural_network.c ${SRC}/train_neural_net.c ${SRC}/imu_data.c ${SRC}/extract_stride_data.c
EXECUTABLES=$(SOURCES:.c=)

all: test_neural_network train_neural_net imu_data train_file_generator extract_stride_data

#collect_neural_net_data: ${SRC}/collect_neural_net_data.c
#	$(CC) $(CFLAGS) -o collect_neural_net_data ${SRC}/collect_neural_net_data.c $(LDFLAGS)

#examine_sensor_data: ${SRC}/examine_sensor_data.c
#	$(CC) $(CFLAGS) -o examine_sensor_data ${SRC}/examine_sensor_data.c $(LDFLAGS)

test_neural_network: ${SRC}/test_neural_network.c
	$(CC) $(CFLAGS) -o test_neural_network ${SRC}/test_neural_network.c $(LDFLAGS)

train_neural_net: ${SRC}/train_neural_net.c
	$(CC) $(CFLAGS) -o train_neural_net ${SRC}/train_neural_net.c $(LDFLAGS)
imu_data: ${SRC}/imu_data.c ${SRC}/LSM9DS0.c
	$(CC) $(CFLAGS) -o imu_data ${SRC}/imu_data.c ${SRC}/LSM9DS0.c $(LDFLAGS)

train_file_generator: ${SRC}/train_file_generator.c
	$(CC) $(CFLAGS) -o train_file_generator ${SRC}/train_file_generator.c $(LDFLAGS)

extract_stride_data: ${SRC}/extract_stride_data.c
	$(CC) $(CFLAGS) -o extract_stride_data ${SRC}/extract_stride_data.c -lm
clean: 
	rm -f collect_neural_net_data examine_sensor_data test_neural_network train_neural_net imu_data train_file_generator extract_stride_data
	rm -f *~
	rm -f TEST.net
	rm -f test_data.txt

CC=gcc
LDFLAGS=-lmraa -lfann
CFLAGS=-Wall -Werror
SOURCES=collect_neural_net_data.c examine_sensor_data.c test_neural_network.c train_neural_net.c imu_data.c extract_stride_data.c
EXECUTABLES=$(SOURCES:.c=)

all: collect_neural_net_data examine_sensor_data test_neural_network train_neural_net imu_data train_file_generator extract_stride_data

collect_neural_net_data: collect_neural_net_data.c
	$(CC) $(CFLAGS) -o collect_neural_net_data collect_neural_net_data.c $(LDFLAGS)

examine_sensor_data: examine_sensor_data.c
	$(CC) $(CFLAGS) -o examine_sensor_data examine_sensor_data.c $(LDFLAGS)

test_neural_network: test_neural_network.c
	$(CC) $(CFLAGS) -o test_neural_network test_neural_network.c $(LDFLAGS)

train_neural_net: train_neural_net.c
	$(CC) $(CFLAGS) -o train_neural_net train_neural_net.c $(LDFLAGS)
imu_data: imu_data.c LSM9DS0.c
	$(CC) $(CFLAGS) -o imu_data imu_data.c LSM9DS0.c $(LDFLAGS)

train_file_generator: train_file_generator.c
	$(CC) $(CFLAGS) -o train_file_generator train_file_generator.c $(LDFLAGS)

extract_stride_data: extract_stride_data.c
	$(CC) $(CFLAGS) -o extract_stride_data extract_stride_data.c -lm
clean: 
	rm -f collect_neural_net_data examine_sensor_data test_neural_network train_neural_net imu_data train_file_generator extract_stride_data
	rm -f *~
	rm -f TEST.net
	rm -f test_data.txt

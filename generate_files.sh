#!/bin/bash


echo "Generate peak, trough, stride, and feature files..."
./extract_stride_data rawdata/data5.csv pt_results/pt5.csv pt_results/stride5.csv pt_results/feature5.csv 8.0 0.05

./extract_stride_data rawdata/data1.csv pt_results/pt1.csv pt_results/stride1.csv pt_results/feature1.csv 1.0 0.6

./extract_stride_data rawdata/data2.csv pt_results/pt2.csv pt_results/stride2.csv pt_results/feature2.csv 2.4 0.03

./extract_stride_data rawdata/data3.csv pt_results/pt3.csv pt_results/stride3.csv pt_results/feature3.csv 4.2 0.01

echo "Generate train files..."
./train_file_generator pt_results/feature5.csv train_file_dir/train5.txt 4 3 4

./train_file_generator pt_results/feature1.csv train_file_dir/train1.txt 1 3 4

./train_file_generator pt_results/feature2.csv train_file_dir/train2.txt 2 3 4

./train_file_generator pt_results/feature3.csv train_file_dir/train3.txt 3 3 4



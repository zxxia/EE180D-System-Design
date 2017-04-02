#!/bin/bash


echo "Generate peak, trough, stride, and feature files..."
./extract_stride_data data_assign10/turn.csv assign10_results/turn_pt.csv assign10_results/stride_turn.csv assign10_results/maxmin_turn.csv assign10_results/feature_turn.csv 50.0 0.7

printf "\n"
./extract_stride_data data_assign10/walk_fast.csv assign10_results/walk_pt.csv assign10_results/stride_walk.csv assign10_results/maxmin_walk.csv assign10_results/feature_walk.csv 50.0 0.7

printf "\n"
./extract_stride_data data_assign10/stair1.csv assign10_results/stair1_pt.csv assign10_results/stride_stair1.csv assign10_results/maxmin_stair1.csv assign10_results/feature_stair1.csv 50.0 0.8

./extract_stride_data data_assign10/stair2.csv assign10_results/stair2_pt.csv assign10_results/stride_stair2.csv assign10_results/maxmin_stair2.csv assign10_results/feature_stair2.csv 50.0 0.7

./extract_stride_data data_assign10/stair3.csv assign10_results/stair3_pt.csv assign10_results/stride_stair3.csv assign10_results/maxmin_stair3.csv assign10_results/feature_stair3.csv 50.0 0.7

cat assign10_results/feature_stair1.csv > assign10_results/feature_stair.csv
tail -n +2 assign10_results/feature_stair2.csv >> assign10_results/feature_stair.csv
tail -n +2 assign10_results/feature_stair3.csv >> assign10_results/feature_stair.csv

printf "\n"
./extract_stride_data data_assign10/turn_test.csv assign10_results/turn_test_pt.csv assign10_results/stride_turn_test.csv assign10_results/maxmin_turn_test.csv assign10_results/feature_turn_test.csv 50.0 0.07

./extract_stride_data data_assign10/walk_fast_test.csv assign10_results/walk_test_pt.csv assign10_results/stride_walk_test.csv assign10_results/maxmin_walk_test.csv assign10_results/feature_walk_test.csv 50.0 0.07

./extract_stride_data data_assign10/stair_test.csv assign10_results/stair_test_pt.csv assign10_results/stride_stair_test.csv assign10_results/maxmin_stair_test.csv assign10_results/feature_stair_test.csv 50.0 0.07



printf "\n"

echo "Generate train files..."
./train_file_generator assign10_results/feature_turn.csv assign10_results/feature_walk.csv assign10_results/feature_stair.csv train_file_dir/train.txt 5 3



echo "Generate test files..."
./train_file_generator assign10_results/feature_turn_test.csv assign10_results/feature_walk_test.csv assign10_results/feature_stair_test.csv train_file_dir/train_test.txt 5 3



#./train_file_generator pt_results/feature3.csv train_file_dir/train3.txt 3 3 4

#./train_file_generator pt_results/test_feature0.csv train_file_dir/test0.txt 1 3 4

#./train_file_generator pt_results/test_feature1.csv train_file_dir/test1.txt 1 3 4

#./train_file_generator pt_results/test_feature2.csv train_file_dir/test2.txt 2 3 4

#./train_file_generator pt_results/test_feature3.csv train_file_dir/test3.txt 3 3 4



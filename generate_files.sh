#!/bin/bash

DATA_DIR='data_assign10'
OUTPUT_DIR='assign10_results'

echo "Stride detection on turning data files..."
./stride_detector $DATA_DIR/turn.csv $OUTPUT_DIR/turn_pt.csv $OUTPUT_DIR/turn_st.csv 50.0

echo "Stride detection on walking data files..."
./stride_detector $DATA_DIR/walk.csv $OUTPUT_DIR/walk_pt.csv $OUTPUT_DIR/walk_st.csv 50.0

echo "Stride detection on stairing data files..."
./stride_detector $DATA_DIR/stair1.csv $OUTPUT_DIR/stair1_pt.csv $OUTPUT_DIR/stair1_st.csv 50.0

./stride_detector $DATA_DIR/stair2.csv $OUTPUT_DIR/stair2_pt.csv $OUTPUT_DIR/stair2_st.csv 50.0

./stride_detector $DATA_DIR/stair3.csv $OUTPUT_DIR/stair3_pt.csv $OUTPUT_DIR/stair3_st.csv 50.0

echo "Feature detection on turning data files..."
./feature_detector $DATA_DIR/turn.csv $OUTPUT_DIR/turn_st.csv $OUTPUT_DIR/turn_maxmin.csv $OUTPUT_DIR/turn_feature.csv
echo "Feature detection on walking data files..."
./feature_detector $DATA_DIR/walk.csv $OUTPUT_DIR/walk_st.csv $OUTPUT_DIR/walk_maxmin.csv $OUTPUT_DIR/walk_feature.csv
echo "Feature detection on stairing data files..."
./feature_detector $DATA_DIR/stair1.csv $OUTPUT_DIR/stair1_st.csv $OUTPUT_DIR/stair1_maxmin.csv $OUTPUT_DIR/stair1_feature.csv

./feature_detector $DATA_DIR/stair2.csv $OUTPUT_DIR/stair2_st.csv $OUTPUT_DIR/stair2_maxmin.csv $OUTPUT_DIR/stair2_feature.csv

./feature_detector $DATA_DIR/stair3.csv $OUTPUT_DIR/stair3_st.csv $OUTPUT_DIR/stair3_maxmin.csv $OUTPUT_DIR/stair3_feature.csv

 cat $OUTPUT_DIR/stair1_feature.csv > $OUTPUT_DIR/stair_feature.csv
 tail -n +2 $OUTPUT_DIR/stair2_feature.csv >> $OUTPUT_DIR/stair_feature.csv
 tail -n +2 $OUTPUT_DIR/stair3_feature.csv >> $OUTPUT_DIR/stair_feature.csv

 printf "\n"
./stride_detector $DATA_DIR/turn_test.csv $OUTPUT_DIR/turn_test_pt.csv $OUTPUT_DIR/turn_test_st.csv 50.0

./stride_detector $DATA_DIR/walk_test.csv $OUTPUT_DIR/walk_test_pt.csv $OUTPUT_DIR/walk_test_st.csv 50.0

./stride_detector $DATA_DIR/stair_test.csv $OUTPUT_DIR/stair_test_pt.csv $OUTPUT_DIR/stair_test_st.csv 50.0

./feature_detector $DATA_DIR/turn_test.csv $OUTPUT_DIR/turn_test_st.csv $OUTPUT_DIR/turn_test_maxmin.csv $OUTPUT_DIR/turn_test_feature.csv

./feature_detector $DATA_DIR/walk_test.csv $OUTPUT_DIR/walk_test_st.csv $OUTPUT_DIR/walk_test_maxmin.csv $OUTPUT_DIR/walk_test_feature.csv

./feature_detector $DATA_DIR/stair_test.csv $OUTPUT_DIR/stair_test_st.csv $OUTPUT_DIR/stair_test_maxmin.csv $OUTPUT_DIR/stair_test_feature.csv



printf "\n"

echo "Generate train files..."
./train_file_generator $OUTPUT_DIR/turn_feature.csv $OUTPUT_DIR/walk_feature.csv $OUTPUT_DIR/stair_feature.csv train_file_dir/train_yiran.txt 17 3



echo "Generate test files..."
./train_file_generator $OUTPUT_DIR/turn_test_feature.csv $OUTPUT_DIR/walk_test_feature.csv $OUTPUT_DIR/stair_test_feature.csv train_file_dir/test_yiran.txt 17 3



#./train_file_generator pt_results/feature3.csv train_file_dir/train3.txt 3 3 4

#./train_file_generator pt_results/test_feature0.csv train_file_dir/test0.txt 1 3 4

#./train_file_generator pt_results/test_feature1.csv train_file_dir/test1.txt 1 3 4

#./train_file_generator pt_results/test_feature2.csv train_file_dir/test2.txt 2 3 4

#./train_file_generator pt_results/test_feature3.csv train_file_dir/test3.txt 3 3 4



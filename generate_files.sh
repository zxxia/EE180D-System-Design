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

# cat assign10_results/feature_stair1.csv > assign10_results/feature_stair.csv
# tail -n +2 assign10_results/feature_stair2.csv >> assign10_results/feature_stair.csv
# tail -n +2 assign10_results/feature_stair3.csv >> assign10_results/feature_stair.csv

# printf "\n"
# ./stride_detector data_assign10/turn_test.csv assign10_results/turn_test_pt.csv assign10_results/stride_turn_test.csv assign10_results/maxmin_turn_test.csv assign10_results/feature_turn_test.csv 50.0 0.07

# ./stride_detector data_assign10/walk_fast_test.csv assign10_results/walk_test_pt.csv assign10_results/stride_walk_test.csv assign10_results/maxmin_walk_test.csv assign10_results/feature_walk_test.csv 50.0 0.07

# ./stride_detector data_assign10/stair_test.csv assign10_results/stair_test_pt.csv assign10_results/stride_stair_test.csv assign10_results/maxmin_stair_test.csv assign10_results/feature_stair_test.csv 50.0 0.07



# printf "\n"

# echo "Generate train files..."
# ./train_file_generator assign10_results/feature_turn.csv assign10_results/feature_walk.csv assign10_results/feature_stair.csv train_file_dir/train.txt 5 3



# echo "Generate test files..."
# ./train_file_generator assign10_results/feature_turn_test.csv assign10_results/feature_walk_test.csv assign10_results/feature_stair_test.csv train_file_dir/train_test.txt 5 3



#./train_file_generator pt_results/feature3.csv train_file_dir/train3.txt 3 3 4

#./train_file_generator pt_results/test_feature0.csv train_file_dir/test0.txt 1 3 4

#./train_file_generator pt_results/test_feature1.csv train_file_dir/test1.txt 1 3 4

#./train_file_generator pt_results/test_feature2.csv train_file_dir/test2.txt 2 3 4

#./train_file_generator pt_results/test_feature3.csv train_file_dir/test3.txt 3 3 4



#!/bin/bash

DATA_DIR='new_data'
OUTPUT_DIR='assign10_results'

echo "Stride detection on turning data files..."
./main $DATA_DIR/turn.csv $OUTPUT_DIR/turn_pt.csv $OUTPUT_DIR/turn_st.csv $OUTPUT_DIR/turn_maxmin.csv $OUTPUT_DIR/turn_feature.csv 50.0

echo "Stride detection on walking data files..."
./main $DATA_DIR/walk.csv $OUTPUT_DIR/walk_pt.csv $OUTPUT_DIR/walk_st.csv $OUTPUT_DIR/walk_maxmin.csv $OUTPUT_DIR/walk_feature.csv 50.0

echo "Stride detection on stairing data files..."
./main $DATA_DIR/stair1.csv $OUTPUT_DIR/stair1_pt.csv $OUTPUT_DIR/stair1_st.csv $OUTPUT_DIR/stair1_maxmin.csv $OUTPUT_DIR/stair1_feature.csv 50.0

./main $DATA_DIR/stair2.csv $OUTPUT_DIR/stair2_pt.csv $OUTPUT_DIR/stair2_st.csv $OUTPUT_DIR/stair2_maxmin.csv $OUTPUT_DIR/stair2_feature.csv 50.0

./main $DATA_DIR/stair3.csv $OUTPUT_DIR/stair3_pt.csv $OUTPUT_DIR/stair3_st.csv $OUTPUT_DIR/stair3_maxmin.csv $OUTPUT_DIR/stair3_feature.csv 50.0


 cat $OUTPUT_DIR/stair1_feature.csv > $OUTPUT_DIR/stair_feature.csv
 tail -n +2 $OUTPUT_DIR/stair2_feature.csv >> $OUTPUT_DIR/stair_feature.csv
 tail -n +2 $OUTPUT_DIR/stair3_feature.csv >> $OUTPUT_DIR/stair_feature.csv

echo "Stride detection on jumping data files..."
./main $DATA_DIR/jump_high.csv $OUTPUT_DIR/jump_high_pt.csv $OUTPUT_DIR/jump_high_st.csv $OUTPUT_DIR/jump_high_maxmin.csv $OUTPUT_DIR/jump_high_feature.csv 50.0

./main $DATA_DIR/jump_med.csv $OUTPUT_DIR/jump_med_pt.csv $OUTPUT_DIR/jump_med_st.csv $OUTPUT_DIR/jump_med_maxmin.csv $OUTPUT_DIR/jump_med_feature.csv 50.0

./main $DATA_DIR/jump_low.csv $OUTPUT_DIR/jump_low_pt.csv $OUTPUT_DIR/jump_low_st.csv $OUTPUT_DIR/jump_low_maxmin.csv $OUTPUT_DIR/jump_low_feature.csv 50.0


 cat $OUTPUT_DIR/jump_high_feature.csv > $OUTPUT_DIR/jump_feature.csv
 tail -n +2 $OUTPUT_DIR/jump_med_feature.csv >> $OUTPUT_DIR/jump_feature.csv
 tail -n +2 $OUTPUT_DIR/jump_low_feature.csv >> $OUTPUT_DIR/jump_feature.csv

echo "Stride detection on running data files..."
./main $DATA_DIR/run.csv $OUTPUT_DIR/run_pt.csv $OUTPUT_DIR/run_st.csv $OUTPUT_DIR/run_maxmin.csv $OUTPUT_DIR/run_feature.csv 50.0



 printf "\n"
./main $DATA_DIR/turn_test.csv $OUTPUT_DIR/turn_test_pt.csv $OUTPUT_DIR/turn_test_st.csv $OUTPUT_DIR/turn_test_maxmin.csv $OUTPUT_DIR/turn_test_feature.csv 50.0

./main $DATA_DIR/walk_test.csv $OUTPUT_DIR/walk_test_pt.csv $OUTPUT_DIR/walk_test_st.csv $OUTPUT_DIR/walk_test_maxmin.csv $OUTPUT_DIR/walk_test_feature.csv 50.0

./main $DATA_DIR/stair_test.csv $OUTPUT_DIR/stair_test_pt.csv $OUTPUT_DIR/stair_test_st.csv $OUTPUT_DIR/stair_test_maxmin.csv $OUTPUT_DIR/stair_test_feature.csv 50.0

./main $DATA_DIR/test_jump_med.csv $OUTPUT_DIR/test_jump_med_pt.csv $OUTPUT_DIR/test_jump_med_st.csv $OUTPUT_DIR/test_jump_med_maxmin.csv $OUTPUT_DIR/test_jump_med_feature.csv 50.0

./main $DATA_DIR/test_run.csv $OUTPUT_DIR/test_run_pt.csv $OUTPUT_DIR/test_run_st.csv $OUTPUT_DIR/test_run_maxmin.csv $OUTPUT_DIR/test_run_feature.csv 50.0



printf "\n"

echo "Generate train files..."
./train_file_generator $OUTPUT_DIR/turn_feature.csv $OUTPUT_DIR/walk_feature.csv $OUTPUT_DIR/stair_feature.csv $OUTPUT_DIR/jump_feature.csv $OUTPUT_DIR/run_feature.csv train_file_dir/train_yiran.txt 17 5



echo "Generate test files..."
./train_file_generator $OUTPUT_DIR/turn_test_feature.csv $OUTPUT_DIR/walk_test_feature.csv $OUTPUT_DIR/stair_test_feature.csv $OUTPUT_DIR/test_jump_med_feature.csv $OUTPUT_DIR/test_run_feature.csv train_file_dir/test_yiran.txt 17 5



#./train_file_generator pt_results/feature3.csv train_file_dir/train3.txt 3 3 4

#./train_file_generator pt_results/test_feature0.csv train_file_dir/test0.txt 1 3 4

#./train_file_generator pt_results/test_feature1.csv train_file_dir/test1.txt 1 3 4

#./train_file_generator pt_results/test_feature2.csv train_file_dir/test2.txt 2 3 4

#./train_file_generator pt_results/test_feature3.csv train_file_dir/test3.txt 3 3 4



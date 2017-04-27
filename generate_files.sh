#!/bin/bash

DATA_DIR='new_data'
OUTPUT_DIR='new_data_results'
THRESHOLD=50
INPUT_NUM=18
OUTPUT_NUM=3
echo "Stride detection and feature selection on turning data files..."
./main $DATA_DIR/turn_left.csv $OUTPUT_DIR/turn_left_pt.csv $OUTPUT_DIR/turn_left_st.csv $OUTPUT_DIR/turn_left_maxmin.csv $OUTPUT_DIR/turn_left_feature.csv $THRESHOLD

./main $DATA_DIR/turn_right.csv $OUTPUT_DIR/turn_right_pt.csv $OUTPUT_DIR/turn_right_st.csv $OUTPUT_DIR/turn_right_maxmin.csv $OUTPUT_DIR/turn_right_feature.csv $THRESHOLD
echo "Stride detection and feature selection on walking data files..."
./main $DATA_DIR/walk_slow.csv $OUTPUT_DIR/walk_slow_pt.csv $OUTPUT_DIR/walk_slow_st.csv $OUTPUT_DIR/walk_slow_maxmin.csv $OUTPUT_DIR/walk_slow_feature.csv $THRESHOLD
./main $DATA_DIR/walk_med.csv $OUTPUT_DIR/walk_med_pt.csv $OUTPUT_DIR/walk_med_st.csv $OUTPUT_DIR/walk_med_maxmin.csv $OUTPUT_DIR/walk_med_feature.csv $THRESHOLD
./main $DATA_DIR/walk_fast.csv $OUTPUT_DIR/walk_fast_pt.csv $OUTPUT_DIR/walk_fast_st.csv $OUTPUT_DIR/walk_fast_maxmin.csv $OUTPUT_DIR/walk_fast_feature.csv $THRESHOLD

echo "Stride detection and feature selection on stairing data files..."
./main $DATA_DIR/stairs_up.csv $OUTPUT_DIR/stairs_up_pt.csv $OUTPUT_DIR/stairs_up_st.csv $OUTPUT_DIR/stairs_up_maxmin.csv $OUTPUT_DIR/stairs_up_feature.csv $THRESHOLD

./main $DATA_DIR/stairs_down.csv $OUTPUT_DIR/stairs_down_pt.csv $OUTPUT_DIR/stairs_down_st.csv $OUTPUT_DIR/stairs_down_maxmin.csv $OUTPUT_DIR/stairs_down_feature.csv $THRESHOLD


# Integrate the feature files
cat $OUTPUT_DIR/walk_slow_feature.csv > $OUTPUT_DIR/walk_feature.csv
tail -n +2 $OUTPUT_DIR/walk_med_feature.csv >> $OUTPUT_DIR/walk_feature.csv
tail -n +2 $OUTPUT_DIR/walk_fast_feature.csv >> $OUTPUT_DIR/walk_feature.csv

cat $OUTPUT_DIR/turn_left_feature.csv > $OUTPUT_DIR/turn_feature.csv
tail -n +2 $OUTPUT_DIR/turn_right_feature.csv >> $OUTPUT_DIR/turn_feature.csv


cat $OUTPUT_DIR/stairs_up_feature.csv > $OUTPUT_DIR/stairs_feature.csv
tail -n +2 $OUTPUT_DIR/stairs_down_feature.csv >> $OUTPUT_DIR/stairs_feature.csv


printf "\n"

echo "Stride detection and feature selection on turning test files..."
./main $DATA_DIR/test_turn_left.csv $OUTPUT_DIR/test_turn_left_pt.csv $OUTPUT_DIR/test_turn_left_st.csv $OUTPUT_DIR/test_turn_left_maxmin.csv $OUTPUT_DIR/test_turn_left_feature.csv $THRESHOLD

./main $DATA_DIR/test_turn_right.csv $OUTPUT_DIR/test_turn_right_pt.csv $OUTPUT_DIR/test_turn_right_st.csv $OUTPUT_DIR/test_turn_right_maxmin.csv $OUTPUT_DIR/test_turn_right_feature.csv $THRESHOLD
echo "Stride detection and feature selection on walking data files..."
./main $DATA_DIR/test_walk_slow.csv $OUTPUT_DIR/test_walk_slow_pt.csv $OUTPUT_DIR/test_walk_slow_st.csv $OUTPUT_DIR/test_walk_slow_maxmin.csv $OUTPUT_DIR/test_walk_slow_feature.csv $THRESHOLD
./main $DATA_DIR/test_walk_med.csv $OUTPUT_DIR/test_walk_med_pt.csv $OUTPUT_DIR/test_walk_med_st.csv $OUTPUT_DIR/test_walk_med_maxmin.csv $OUTPUT_DIR/test_walk_med_feature.csv $THRESHOLD
./main $DATA_DIR/test_walk_fast.csv $OUTPUT_DIR/test_walk_fast_pt.csv $OUTPUT_DIR/test_walk_fast_st.csv $OUTPUT_DIR/test_walk_fast_maxmin.csv $OUTPUT_DIR/test_walk_fast_feature.csv $THRESHOLD

echo "Stride detection and feature selection on stairing test files..."
./main $DATA_DIR/test_stairs_up.csv $OUTPUT_DIR/test_stairs_up_pt.csv $OUTPUT_DIR/test_stairs_up_st.csv $OUTPUT_DIR/test_stairs_up_maxmin.csv $OUTPUT_DIR/test_stairs_up_feature.csv $THRESHOLD

./main $DATA_DIR/test_stairs_down.csv $OUTPUT_DIR/test_stairs_down_pt.csv $OUTPUT_DIR/test_stairs_down_st.csv $OUTPUT_DIR/test_stairs_down_maxmin.csv $OUTPUT_DIR/test_stairs_down_feature.csv $THRESHOLD



# Integrate the feature files
cat $OUTPUT_DIR/test_walk_slow_feature.csv > $OUTPUT_DIR/test_walk_feature.csv
tail -n +2 $OUTPUT_DIR/test_walk_med_feature.csv >> $OUTPUT_DIR/test_walk_feature.csv
tail -n +2 $OUTPUT_DIR/test_walk_fast_feature.csv >> $OUTPUT_DIR/test_walk_feature.csv

cat $OUTPUT_DIR/test_turn_left_feature.csv > $OUTPUT_DIR/test_turn_feature.csv
tail -n +2 $OUTPUT_DIR/turn_right_feature.csv >> $OUTPUT_DIR/test_turn_feature.csv


cat $OUTPUT_DIR/test_stairs_up_feature.csv > $OUTPUT_DIR/test_stairs_feature.csv
tail -n +2 $OUTPUT_DIR/test_stairs_down_feature.csv >> $OUTPUT_DIR/test_stairs_feature.csv


printf "\n"

printf "\nGenerate train files...\n"
./train_file_generator $OUTPUT_DIR/turn_feature.csv $OUTPUT_DIR/walk_feature.csv $OUTPUT_DIR/stairs_feature.csv $OUTPUT_DIR/train.txt $INPUT_NUM $OUTPUT_NUM



printf "\nGenerate test files...\n"
./train_file_generator $OUTPUT_DIR/test_turn_feature.csv $OUTPUT_DIR/test_walk_feature.csv $OUTPUT_DIR/test_stairs_feature.csv $OUTPUT_DIR/test.txt $INPUT_NUM $OUTPUT_NUM

printf "\nTrain neural network...\n"
./train_neural_net $OUTPUT_DIR/train.txt $INPUT_NUM $OUTPUT_NUM

printf "\nTest neural network...\n"
./test_neural_network $OUTPUT_DIR/test.txt

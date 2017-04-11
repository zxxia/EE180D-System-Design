#!/bin/bash

DATA_DIR='rawdata'
OUTPUT_DIR='pt_results'

echo "Stride detection on Walk1 data files..."
./stride_detector $DATA_DIR/data1.csv $OUTPUT_DIR/data1_pt.csv $OUTPUT_DIR/data1_st.csv 50.0

echo "Stride detection on Walk2 data files..."
./stride_detector $DATA_DIR/data2.csv $OUTPUT_DIR/data2_pt.csv $OUTPUT_DIR/data2_st.csv 50.0

echo "Stride detection on Walk3 data files..."
./stride_detector $DATA_DIR/data3.csv $OUTPUT_DIR/data3_pt.csv $OUTPUT_DIR/data3_st.csv 50.0

echo "Feature detection on turning data files..."
./feature_detector $DATA_DIR/data1.csv $OUTPUT_DIR/data1_st.csv $OUTPUT_DIR/data1_maxmin.csv $OUTPUT_DIR/data1_feature.csv
echo "Feature detection on walking data files..."
./feature_detector $DATA_DIR/data2.csv $OUTPUT_DIR/data2_st.csv $OUTPUT_DIR/data2_maxmin.csv $OUTPUT_DIR/data2_feature.csv
echo "Feature detection on stairing data files..."
./feature_detector $DATA_DIR/data3.csv $OUTPUT_DIR/data3_st.csv $OUTPUT_DIR/data3_maxmin.csv $OUTPUT_DIR/data3_feature.csv

 #cat $OUTPUT_DIR/stair1_feature.csv > $OUTPUT_DIR/stair_feature.csv
 #tail -n +2 $OUTPUT_DIR/stair2_feature.csv >> $OUTPUT_DIR/stair_feature.csv
 #tail -n +2 $OUTPUT_DIR/stair3_feature.csv >> $OUTPUT_DIR/stair_feature.csv

 printf "\n"
./stride_detector $DATA_DIR/test1.csv $OUTPUT_DIR/test1_pt.csv $OUTPUT_DIR/test1_st.csv 50.0

./stride_detector $DATA_DIR/test2.csv $OUTPUT_DIR/test2_pt.csv $OUTPUT_DIR/test2_st.csv 50.0

./stride_detector $DATA_DIR/test3.csv $OUTPUT_DIR/test3_pt.csv $OUTPUT_DIR/test3_st.csv 50.0

./feature_detector $DATA_DIR/test1.csv $OUTPUT_DIR/test1_st.csv $OUTPUT_DIR/test1_maxmin.csv $OUTPUT_DIR/test1_feature.csv

./feature_detector $DATA_DIR/test2.csv $OUTPUT_DIR/test2_st.csv $OUTPUT_DIR/test2_maxmin.csv $OUTPUT_DIR/test2_feature.csv

./feature_detector $DATA_DIR/test3.csv $OUTPUT_DIR/test3_st.csv $OUTPUT_DIR/test3_maxmin.csv $OUTPUT_DIR/test3_feature.csv



printf "\n"

echo "Generate train files..."
./train_file_generator $OUTPUT_DIR/data1_feature.csv $OUTPUT_DIR/data2_feature.csv $OUTPUT_DIR/data3_feature.csv train_file_dir/train_yiran.txt 17 3



echo "Generate test files..."
./train_file_generator $OUTPUT_DIR/test1_feature.csv $OUTPUT_DIR/test2_feature.csv $OUTPUT_DIR/test3_feature.csv train_file_dir/test_yiran.txt 17 3

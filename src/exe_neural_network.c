#include "exe_neural_network.h"

void init_networks()
{
    global_ann = fann_create_from_file("GLOBAL.net");
    turn_ann = fann_create_from_file("TURN.net");
}

void destroy_networks()
{
    fann_destroy(global_ann);
    fann_destroy(turn_ann);
}

int exe_global_neural_network(const GlobalFeature *global_feature)
{
    int i;
    int location;

    double max;
    fann_type *calc_out;
    fann_type input[18];
   
    max = -100;
    /* parse the feature data*/

    input[0] = global_feature->accel_y_seg0_max/6.0; 
    input[1] = global_feature->accel_y_seg0_min/6.0;
    input[2] = global_feature->accel_y_seg1_max/6.0;
    input[3] = global_feature->accel_y_seg1_min/6.0;
    input[4] = global_feature->accel_y_seg2_max/6.0;
    input[5] = global_feature->accel_y_seg2_min/6.0;
    input[6] = global_feature->accel_y_seg3_max/6.0;
    input[7] = global_feature->accel_y_seg3_min/6.0;
    input[8] = global_feature->gyro_y_seg0_max/500.0;
    input[9] = global_feature->gyro_y_seg0_min/500.0;
    input[10] = global_feature->gyro_y_seg1_max/500.0;
    input[11] = global_feature->gyro_y_seg1_min/500.0;
    input[12] = global_feature->gyro_y_seg2_max/500.0;
    input[13] = global_feature->gyro_y_seg2_min/500.0;
    input[14] = global_feature->gyro_y_seg3_max/500.0;
    input[15] = global_feature->gyro_y_seg3_min/500.0;
    input[16] = global_feature->gyro_y_abs_integral/176.690486;
    input[17] = global_feature->period/3.272060;

    /*Caluculate the type predicted by our trained network*/
    calc_out = fann_run(global_ann, input);
    for (i = 0; i < 4; i++) {
        if (calc_out[i] > max) {
            max = calc_out[i];
            location = i;
        }
    }
    return location;
}

int exe_turn_neural_network(const TurnFeature *turn_feature)
{
    int i;
    int location;

    double max;
    fann_type *calc_out;
    fann_type input[9];
   
    max = -100;
    /* parse the feature data*/


    input[0] = turn_feature->gyro_y_seg0_max/500.0;
    input[1] = turn_feature->gyro_y_seg0_min/500.0;
    input[2] = turn_feature->gyro_y_seg1_max/500.0;
    input[3] = turn_feature->gyro_y_seg1_min/500.0;
    input[4] = turn_feature->gyro_y_seg2_max/500.0;
    input[5] = turn_feature->gyro_y_seg2_min/500.0;
    input[6] = turn_feature->gyro_y_seg3_max/500.0;
    input[7] = turn_feature->gyro_y_seg3_min/500.0;
    input[8] = turn_feature->gyro_y_integral/176.690486;


    /*Caluculate the type predicted by our trained network*/
    calc_out = fann_run(turn_ann, input);
    for (i = 0; i < 2; i++) {
        if (calc_out[i] > max) {
            max = calc_out[i];
            location = i;
        }
    }
    return location;
}
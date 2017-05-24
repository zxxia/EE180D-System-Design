#include "exe_neural_network.h"

void init_networks()
{
    global_ann = fann_create_from_file("net/GLOBAL.net");
    turn_ann = fann_create_from_file("net/TURN.net");
    walk_ann = fann_create_from_file("net/WALK.net");
    stair_ann = fann_create_from_file("net/STAIRS.net");
}

void destroy_networks()
{
    fann_destroy(global_ann);
    fann_destroy(turn_ann);
    fann_destroy(walk_ann);
    fann_destroy(stair_ann);
}

int exe_global_neural_network(const GlobalFeature *feature)
{
    int i;
    int location;

    double max;
    fann_type *calc_out;
    fann_type input[10];
   
    max = -100;
    /* parse the feature data*/

    input[0] = feature->accel_y_seg0_max/6.0; 
    input[1] = feature->accel_y_seg0_min/6.0;
    input[2] = feature->accel_y_seg1_max/6.0;
    input[3] = feature->accel_y_seg1_min/6.0;
    input[4] = feature->accel_y_seg2_max/6.0;
    input[5] = feature->accel_y_seg2_min/6.0;
    input[6] = feature->accel_y_seg3_max/6.0;
    input[7] = feature->accel_y_seg3_min/6.0;
    input[8] = feature->gyro_y_abs_integral/176.690486;
    input[9] = feature->period/3.272060;

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

int exe_turn_neural_network(const TurnFeature *feature)
{
    int i;
    int location;

    double max;
    fann_type *calc_out;
    fann_type input[9];
   
    max = -100;
    /* parse the feature data*/


    input[0] = feature->gyro_y_seg0_max/500.0;
    input[1] = feature->gyro_y_seg0_min/500.0;
    input[2] = feature->gyro_y_seg1_max/500.0;
    input[3] = feature->gyro_y_seg1_min/500.0;
    input[4] = feature->gyro_y_seg2_max/500.0;
    input[5] = feature->gyro_y_seg2_min/500.0;
    input[6] = feature->gyro_y_seg3_max/500.0;
    input[7] = feature->gyro_y_seg3_min/500.0;
    input[8] = feature->gyro_y_integral/176.690486;

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


int exe_walk_neural_network(const WalkFeature *feature)
{
    int i;
    int location;

    double max;
    fann_type *calc_out;
    fann_type input[13];
   
    max = -100;
    /* parse the feature data*/


    input[0] = feature->accel_x_seg0_max/6.0;
    input[1] = feature->accel_x_seg0_min/6.0;
    input[2] = feature->accel_x_seg0_rms/6.0;

    input[3] = feature->accel_x_seg1_max/6.0;
    input[4] = feature->accel_x_seg1_min/6.0;
    input[5] = feature->accel_x_seg1_rms/6.0;

    input[6] = feature->accel_x_seg2_max/6.0;
    input[7] = feature->accel_x_seg2_min/6.0;
    input[8] = feature->accel_x_seg2_rms/6.0;

    input[9] = feature->accel_x_seg3_max/6.0;
    input[10] = feature->accel_x_seg3_min/6.0;
    input[11] = feature->accel_x_seg3_rms/6.0;

    input[12] = feature->period/3.272060;


    /*Caluculate the type predicted by our trained network*/
    calc_out = fann_run(walk_ann, input);
    for (i = 0; i < 3; i++) {
        if (calc_out[i] > max) {
            max = calc_out[i];
            location = i;
        }
    }
    return location;
}


int exe_stair_neural_network(const StairFeature *feature)
{
    int i;
    int location;

    double max;
    fann_type *calc_out;
    fann_type input[16];
   
    max = -100;
    /* parse the feature data*/


    input[0] = feature->accel_x_seg0_max/6.0;
    input[1] = feature->accel_x_seg0_min/6.0;
    input[2] = feature->accel_x_seg1_max/6.0;
    input[3] = feature->accel_x_seg1_min/6.0;
    input[4] = feature->accel_x_seg2_max/6.0;
    input[5] = feature->accel_x_seg2_min/6.0;
    input[6] = feature->accel_x_seg3_max/6.0;
    input[7] = feature->accel_x_seg3_min/6.0;

    input[8] = feature->accel_y_seg0_max/6.0;
    input[9] = feature->accel_y_seg0_min/6.0;
    input[10] = feature->accel_y_seg1_max/6.0;
    input[11] = feature->accel_y_seg1_min/6.0;
    input[12] = feature->accel_y_seg2_max/6.0;
    input[13] = feature->accel_y_seg2_min/6.0;
    input[14] = feature->accel_y_seg3_max/6.0;
    input[15] = feature->accel_y_seg3_min/6.0;

    /*Caluculate the type predicted by our trained network*/
    calc_out = fann_run(stair_ann, input);
    for (i = 0; i < 2; i++) {
        if (calc_out[i] > max) {
            max = calc_out[i];
            location = i;
        }
    }
    return location;
}

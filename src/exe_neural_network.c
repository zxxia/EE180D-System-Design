#include "exe_neural_network.h"

void init_networks()
{
    global_ann = fann_create_from_file("GLOBAL.net");
}

void destroy_networks()
{
    fann_destroy(global_ann);
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

    /*switch(location) {
        case TURN_CASE:
            printf("Got Input values -> Movement type is %s\n", "Turning");
            return TURN_CASE;
        case WALK_CASE:
            printf("Got Input values -> Movement type is %s\n", "Walking");
            return WALK_CASE;
        case STAIR_CASE:
            printf("Got Input values -> Movement type is %s\n", "Stairs");
            return STAIR_CASE;
        case RUN_CASE:
            return RUN_CASE;
        case JUMP_CASE:
            printf("Got Input values -> Movement type is %s\n", "Jumping");
            return JUMP_CASE;
    }*/
}

int exe_walk_neural_network(const char *walk_feature_file)
{
    int i;
    int location;

    float max;
    fann_type *calc_out;
    fann_type input[9];
    struct fann *ann;

    FILE *fp;
    fp = fopen(walk_feature_file, "r+");
    //int answer[3] = {-1,-1,-1};
    int numLines = 0;
    int inN = 0;
    int outN = 0;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int rv;
    //char* str_answer;
    
    ann = fann_create_from_file("WALK.net");


    /* get number of lines in the file */
    read = getline(&line, &len, fp);
    rv = sscanf(line, "%d\t%d\t%d\n", &numLines, &inN, &outN);
    if (rv != 3) {
        fprintf(stderr,"Error: Failed to read header line\n");
        exit(EXIT_FAILURE);
    }

    /* start reading the data from the file into the data structures */
    i = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
        max = -100;
        /* parse the feature data*/
        rv = sscanf(line, "%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n", 
            &input[0], &input[1], &input[2],
            &input[3], &input[4], &input[5],
            &input[6], &input[7], &input[8]);
        if (rv != 9) {
            fprintf(stderr,"Error: Failed to read data line\n");
            exit(EXIT_FAILURE);
        }
        /*Caluculate the type predicted by our trained network*/
        calc_out = fann_run(ann, input);
        for (i = 0; i < 3; i++) {
            if (calc_out[i] > max) {
                max = calc_out[i];
                location = i;
            }
        }

        switch(location) {
            case FAST_WALK:
                printf("\t\tGot Input values -> Walking type is %s\n", "Fast walking");
                sleep(1);
                break;
            case MED_WALK:
                printf("\t\tGot Input values -> Walking type is %s\n", "Med walking");
                sleep(1);
                break;
            case SLOW_WALK:
                printf("\t\tGot Input values -> Walking type is %s\n", "Slow walking");
                sleep(1);
                break;
        }
    }

    fclose(fp);
    fann_destroy(ann);
    return 0;
}
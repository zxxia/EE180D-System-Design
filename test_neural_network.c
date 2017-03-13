#include <unistd.h>
#include <mraa/aio.h>
#include <stdio.h>
#include "floatfann.h"

int main()
{
    int i;
    int location;
    //int temp0, temp1, temp2, location;
    //uint16_t value0, value1, value2;
    float max;
    fann_type *calc_out;
    fann_type input[3];
    struct fann *ann;
    //mraa_aio_context lightsensor0, lightsensor1, lightsensor2;
    //FILE *fp;
    //fp = fopen("test_result.txt", "r+");
    
    ann = fann_create_from_file("TEST.net");
/*
    lightsensor0 = mraa_aio_init(0);
    lightsensor1 = mraa_aio_init(1);
    lightsensor2 = mraa_aio_init(2);*/

    while (1) {
        max = -100;
        scanf("Inupt1: %f\n", &input[0]);
        scanf("Inupt2: %f\n", &input[1]);
        scanf("Inupt3: %f\n", &input[2]);     
        calc_out = fann_run(ann, input);

        for (i = 0; i < 3; i++) {
            if (calc_out[i] > max) {
                max = calc_out[i];
                location = i;
            }
        }

	printf("Input values: %f, %f, %f -> Movement type is %d\n", input[0], input[1], input[2], location);
        sleep(1);
    }
    fann_destroy(ann);
    return 0;
}

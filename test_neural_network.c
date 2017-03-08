#include <unistd.h>
#include <mraa/aio.h>
#include <stdio.h>
#include "floatfann.h"

int main()
{
    int i;
    int temp0, temp1, temp2, location;
    uint16_t value0, value1, value2;
    float max;
    fann_type *calc_out;
    fann_type input[3];
    struct fann *ann;
    mraa_aio_context lightsensor0, lightsensor1, lightsensor2;
    
    ann = fann_create_from_file("TEST.net");

    lightsensor0 = mraa_aio_init(0);
    lightsensor1 = mraa_aio_init(1);
    lightsensor2 = mraa_aio_init(2);

    while (1) {
        
        temp0 = 0;
        temp1 = 0;
        temp2 = 0;
        max = -100;

        for (i = 0; i < 50; i++) {
            temp0 += mraa_aio_read(lightsensor0);
            temp1 += mraa_aio_read(lightsensor1);
            temp2 += mraa_aio_read(lightsensor2);
            usleep(10000);
        }

        value0 = temp0 / 50;
        value1 = temp1 / 50;
        value2 = temp2 / 50;

        input[0] = (float) value0 / 1000;
        input[1] = (float) value1 / 1000;
        input[2] = (float) value2 / 1000;
        calc_out = fann_run(ann, input);

        for (i = 0; i < 5; i++) {
            if (calc_out[i] > max) {
                max = calc_out[i];
                location = i;
            }
        }

	printf("Light sensor values: %d, %d, %d -> location is %d\n", value0, value1, value2, location);
        sleep(1);
    }
    fann_destroy(ann);
    return 0;
}

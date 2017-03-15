#include <unistd.h>
#include <mraa/aio.h>
#include <stdio.h>
#include "floatfann.h"

int main(int argc, char **argv)
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
    FILE *fp;
    fp = fopen(argv[1], "r+");

    int conf_matrix[4][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
    int answer[4] = {-1,-1,-1,-1};
    int answerLoc;
    int numLines = 0;
    int inN = 0;
    int outN = 0;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int rv;
    
    ann = fann_create_from_file("TEST.net");

    fprintf(stderr,"Alive1\n");
    /* count the number of lines in the file */
    read = getline(&line, &len, fp);
    rv = sscanf(line, "%d\t%d\t%d\n", &numLines, &inN, &outN);
    if (rv != 3) {
        fprintf(stderr,"Failed to read line1, %d\n",rv);
        exit(EXIT_FAILURE);
    }

    /* start reading the data from the file into the data structures */
    i = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
        max = -100;
        fprintf(stderr,"Alive2\n");
        /* parse the data 1*/
        rv = sscanf(line, "%f\t%f\t%f\n", &input[0], &input[1], &input[2]);
        if (rv != 3) {
            fprintf(stderr,"Failed to read line2");
            exit(EXIT_FAILURE);
        }
        calc_out = fann_run(ann, input);
        for (i = 0; i < 4; i++) {
            if (calc_out[i] > max) {
                max = calc_out[i];
                location = i;
            }
        }

        fprintf(stderr,"Alive3\n");
        read = getline(&line, &len, fp);
        /* parse the data 2*/
        rv = sscanf(line, "%d\t%d\t%d\t%d\n", &answer[0], &answer[1], &answer[2],&answer[3]);
        if (rv != 4) {
            fprintf(stderr,"Failed to read line3");
            exit(EXIT_FAILURE);
        }
        fprintf(stderr,"Alive4\n");
        for(i=0; i<4; i++) {
            if(answer[i] == 1) {
                answerLoc = i;
                break;
            }
        }
        fprintf(stderr,"Alive5\nAnswerLoc: %d\nlocation: %d\n", answerLoc, location);
        conf_matrix[answerLoc][location]++;

        printf("Input values: %f, %f, %f -> Movement type is %d\n", input[0], input[1], input[2], location);
        sleep(1);
    }
    fclose(fp);
    fann_destroy(ann);
    return 0;
}





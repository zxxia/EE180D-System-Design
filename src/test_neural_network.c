#include <unistd.h>
#include <mraa/aio.h>
#include <stdio.h>
#include "floatfann.h"

#define INPUT_NEURON_NUM 18
#define OUTPUT_NEURON_NUM 4

int main(int argc, char **argv)
{
    int i;
    int location;

    float max;
    fann_type *calc_out;
    fann_type input[INPUT_NEURON_NUM];
    struct fann *ann;

    FILE *fp;
    fp = fopen(argv[1], "r+");
    //There are total four types of movement which means we will have a 4*4 confusion matix
    // turn, walk, stairs, run
    int conf_matrix[OUTPUT_NEURON_NUM][OUTPUT_NEURON_NUM] = {{0,0,0,},{0,0,0},{0,0,0}, {0,0,0}};
    int answer[OUTPUT_NEURON_NUM] = {-1,-1,-1,-1};
    int answerLoc;
    int numLines = 0;
    int inN = 0;
    int outN = 0;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int rv;
    
    ann = fann_create_from_file("TEST.net");


    /* get number of lines in the file */
    read = getline(&line, &len, fp);
    rv = sscanf(line, "%d\t%d\t%d\n", &numLines, &inN, &outN);
    if (rv != 3) {
        exit(EXIT_FAILURE);
    }

    /* start reading the data from the file into the data structures */

    while ((read = getline(&line, &len, fp)) != -1) {
        max = -100;
        /* parse the feature data*/
        rv = sscanf(line, "%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n", 
            &input[0], &input[1], &input[2],&input[3],&input[4],
            &input[5], &input[6], &input[7],&input[8],&input[9],
            &input[10], &input[11], &input[12],&input[13],&input[14],
            &input[15], &input[16], &input[17]);
        if (rv != INPUT_NEURON_NUM) {
            fprintf(stderr,"Failed to read line2");
            exit(EXIT_FAILURE);
        }
        /*Caluculate the type predicted by our trained network*/
        calc_out = fann_run(ann, input);
        for (i = 0; i < OUTPUT_NEURON_NUM; i++) {
            if (calc_out[i] > max) {
                max = calc_out[i];
                location = i;
            }
        }

        read = getline(&line, &len, fp);
        /* parse the type data in the test file*/
        rv = sscanf(line, "%d\t%d\t%d\t%d\n", &answer[0], &answer[1], &answer[2], &answer[3]);
        if (rv != OUTPUT_NEURON_NUM) {
            fprintf(stderr,"Failed to read line3");
            exit(EXIT_FAILURE);
        }
        /*Get the expected type*/
        for(i = 0; i < OUTPUT_NEURON_NUM; i++) {
            if(answer[i] == 1) {
                answerLoc = i;
                break;
            }
        }
        /*Add this type in certain position in confusion matrix*/
        conf_matrix[answerLoc][location]++;

        printf("Input values: %f, %f, %f, %f, %f -> Movement type is %d\n", input[0], input[1], input[2], input[3], input[4], location);
        usleep(500000);
    }

    /*Display the confusion matrix in std::output*/
    int row;
    int col;
    for(row = 0; row < OUTPUT_NEURON_NUM; row++) {
        for(col = 0; col < OUTPUT_NEURON_NUM; col++) {
            printf("%d\t", conf_matrix[row][col]);
        }
        printf("\n");
    }

    fclose(fp);
    fann_destroy(ann);
    return 0;
}







int main(int argc, char **argv)
{
    int i;
    int location;

    float max;
    fann_type *calc_out;
    fann_type input[17];
    struct fann *ann;

    FILE *fp;
    fp = fopen(argv[1], "r+");
    //There are total three types of movement which means we will have a 3*3 confusion matix
    int conf_matrix[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
    int answer[3] = {-1,-1,-1};
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
    i = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
        max = -100;
        /* parse the feature data*/
        rv = sscanf(line, "%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n", 
            &input[0], &input[1], &input[2],&input[3],&input[4],
            &input[5], &input[6], &input[7],&input[8],&input[9],
            &input[10], &input[11], &input[12],&input[13],&input[14],
            &input[15], &input[16]);
        if (rv != 17) {
            fprintf(stderr,"Failed to read line2");
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

        read = getline(&line, &len, fp);
        /* parse the type data in the test file*/
        rv = sscanf(line, "%d\t%d\t%d\n", &answer[0], &answer[1], &answer[2]);
        if (rv != 3) {
            fprintf(stderr,"Failed to read line3");
            exit(EXIT_FAILURE);
        }
        /*Get the expected type*/
        for(i=0; i<3; i++) {
            if(answer[i] == 1) {
                answerLoc = i;
                break;
            }
        }
        /*Add this type in certain position in confusion matrix*/
        conf_matrix[answerLoc][location]++;

        printf("Input values: %f, %f, %f, %f, %f -> Movement type is %d\n", input[0], input[1], input[2], input[3], input[4], location);
        sleep(1);
    }

    /*Display the confusion matrix in std::output*/
    int row;
    int col;
    for(row = 0; row < 3; row++) {
        for(col = 0; col < 3; col++) {
            printf("%d\t", conf_matrix[row][col]);
        }
        printf("\n");
    }

    fclose(fp);
    fann_destroy(ann);
    return 0;
}





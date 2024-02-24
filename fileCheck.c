#include "config.h"

int main(int argc, char** argv){
    char buffer[3672];
    FILE* trial;
    FILE* sample;

    trial = fopen("trial.txt", "w");
    sample = fopen("sampleExe.txt", "r");

    //fseek(sample, 0, SEEK_END);    
    int readbytes = fread(buffer, sizeof(char), 3672, sample);
    printf("%d bytes were read into buffer\n", readbytes);

    int writtenbytes = fputs(buffer, trial);
    
    fclose(sample);
    fclose(trial);
    return 0;
}
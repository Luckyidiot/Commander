#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void File_Naming(char* filename, const char* IPaddr){
    /**
     * Each client will have an execution file
     * Those files will be named using their IP address and Date & Time to ensure the uniqueness
    */
    time_t rawTime = time(NULL);
    sprintf(filename, "%s_%ld", IPaddr, rawTime);
    
}

int main(int argc, char** argv){
    char filename[50];


    time_t rawTime = time(NULL);
    printf("Unformatter: %ld\n", rawTime);

    char* formattedTime = ctime(&rawTime);
    printf("Formatter: %s\n", formattedTime);

    char* IPaddr = "127.0.0.1";
    File_Naming(filename, IPaddr);

    printf("The filename is: %s\n", filename);
    return 0;
}
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void){
    pid_t ID = fork();
    if (ID == 0){
        printf("This is children\n");
        printf("This is children\n");
    }
    else {
        printf("This is parent\n");
    }
}
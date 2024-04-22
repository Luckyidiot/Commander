#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

void Process(){
    volatile sig_atomic_t terminate = 1;

    void handler(int signum){
        terminate = 0;
    }

    signal(SIGALRM, handler);
    alarm(10);

    while (terminate){
        printf("Scanning....\n");
        sleep(2);
    }
    
    printf("%d\n", terminate);
}


int main(void){
    Process();
    return 0; 
}




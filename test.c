#include <stdio.h>
#include <time.h>
#include <sys/timeb.h>
#include <unistd.h>
#define deltaTime 1

int main(){
    // int gettime = (int)time(NULL);
    // int getclock = (int)clock();
    // while(1) {
    //     if(gettime != (int)time(NULL)) {
    //         gettime = (int)time(NULL);
    //         int currclock = (int)clock();
    //         printf("%d\n", currclock - getclock);
    //         getclock = currclock;
    //     }
    //     usleep(deltaTime * 1000);
    // }
    while(1) {
        printf("%d\n");
        usleep(deltaTime * 1000);
    }
    return 0;
}
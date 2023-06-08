#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

//for get input
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

#include <signal.h>

#include <stdlib.h>


int main(){
    pid_t pid =fork();
    int childstat;
    execl("/home/chek/syspro/system-programming9/dicetest","dicetest",NULL);
    if(pid==0){
        execl("/home/chek/syspro/system-programming9/dicetest","dicetest",NULL);
    }
    else{
        wait(&childstat);
        printf("%d  child end\n",childstat);
        printf("%d \n",WEXITSTATUS(childstat));
        printf("this is the parae\n");
        // exit(123);
    }
}


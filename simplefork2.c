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
    // printf(strlen("./cross"));
    // printf(strlen(scrcat"./"));
    execlp("./cross",NULL);
}

// #include <stdio.h>
// #include <unistd.h>

// int main()
// {
//    execlp( "ls", "ls", "-al", "/tmp", NULL);

//    printf( "이 메시지가 보이면 지정된 프로그램이 \
// 없거나 어떤 문제로 실행되지 못한 것입니다.\n");
// }
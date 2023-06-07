#include "./libcube/coroutine.h"
#include "./libcube/cube.h"

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

int buf[6]={0,0,0,0,0,0};
void* ptr;
const char* name = "input";

void int_handler(int sig)
{
    printf("Process %d received signal %d\n", getpid(), sig);
    memcpy(buf,ptr,sizeof(int)*4);
    ButtonDown(buf[1],buf[2]);
    
}
void getinput(){
    buf[0]=getpid();
    signal(SIGUSR1,int_handler);
    /* the size (in bytes) of shared memory object */
    const int SIZE = sizeof(int)*10;
 
    /* name of the shared memory object */
    
    int shm_fd;
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    //init the memory
    ftruncate(shm_fd, SIZE);
    /* memory map the shared memory object */
    ptr = mmap(0, SIZE,  PROT_READ|PROT_WRITE,MAP_SHARED, shm_fd, 0);
    memcpy(ptr,buf,sizeof(int)*2);

    // munmap(ptr,SIZE);
    // ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    
}



int main() {
    
    StartCubeRoutine();
    SetColor(1,1,MakeColor(100,100,100));

    return 0;
}
int mode=1;
void ButtonDown(int face, int lednum)
{
    if (mode==0){
        shm_unlink(name);
        exit(0);
    }
    SetColor(0, 2, MakeRandomColor());
    ChangeColorImm();
    if(face==0 &&lednum==7){
        CubeInit();
        ChangeColorImm();
        mode=0;
        // exit(333);
    }
    if(face==0 &&lednum==4){
            pid_t pid =fork();
    int childstat;
    if(pid==0){
        execl("/home/chek/syspro/system-programming9/dicetest","dicetest",NULL);
        // printf("child\n");
        // exit(1234);

    }
    else{
        wait(&childstat);
        printf("%d  child end\n",childstat);
        printf("%d \n",WEXITSTATUS(childstat));
        printf("this is the parae\n");
        // exit(123);
    }
    }
    // // 눌린 버튼 색을 무작위로 바꾸는 애플리케이션
    // Color color;
    // int choice = rand() % 3;
    // color.r = choice == 0 ? 255 : 0;
    // color.g = choice == 1 ? 255 : 0;
    // color.b = choice == 2 ? 255 : 0;
    // SetColor(face, lednum, color);
    // StartFadeColorCoroutine(FadeColor, face, lednum, 3000);

    /* ---------------------------------------------------------------------------------------*/
}

void ButtonUp(int face, int lednum)
{
    /* --------- 여기에 몇 번 face의 몇 번 버튼이 떼어졌을 때 어떤 동작을 수행할지를 작성 ---------*/

    

    /* ---------------------------------------------------------------------------------------*/
}

// Button Input을 여기에 구현
void pressButtonsAutomatically(Coroutine *coroutine) {
    BEGIN_COROUTINE(coroutine);

    // 마구잡이로 아무 버튼이나 클릭하는 무한루프...
    while (1) {
        ButtonDown(rand() % 6, rand() % 9);
        WAIT_FOR_MILISEC(coroutine, 1000);
    }

    END_COROUTINE(coroutine);
}

// 프로그램이 시작되면 최초 한 번 실행된다.
void Start()
{
    getinput();
    // pid_t pid =fork();
    // int childstat;
    // if(pid==0){
    //     execl("/home/chek/syspro/system-programming9/dicetest","dicetest",NULL);
    //     // printf("child\n");
    //     exit(1234);

    // }
    // else{
    //     wait(&childstat);
    //     printf("%d  child end\n",childstat);
    //     printf("%d \n",WEXITSTATUS(childstat));
    //     printf("this is the parae\n");
    //     // exit(123);
    // }
    // StartCoroutine(pressButtonsAutomatically);
}

// 매 Timing Sequence (deltaTime ms) 마다 실행된다.
void Update()
{
    int a;
}
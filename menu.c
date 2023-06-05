#include "./libcube/coroutine.h"
#include "./libcube/cube.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    StartCubeRoutine();
    SetColor(1,1,MakeColor(100,100,100));

    return 0;
}

void ButtonDown(int face, int lednum)
{
    SetColor(0, 2, MakeRandomColor());
    ChangeColorImm();
    if(face==0 &&lednum==7){
        void CubeInit();
        ChangeColorImm();
        exit(333);
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
    pid_t pid =fork();
    int childstat;
    if(pid==0){
        execl("/home/chek/syspro/system-programming9/dicetest","dicetest",NULL);
        // printf("child\n");
        exit(1234);

    }
    else{
        wait(&childstat);
        printf("%d  child end\n",childstat);
        printf("%d \n",WEXITSTATUS(childstat));
        printf("this is the parae\n");
        // exit(123);
    }
    // StartCoroutine(pressButtonsAutomatically);
}

// 매 Timing Sequence (deltaTime ms) 마다 실행된다.
void Update()
{
    int a;
}
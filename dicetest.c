#include "./libcube/coroutine.h"
#include "./libcube/cube.h"

#include <time.h>
int main() {
    StartCubeRoutine();
    SetColor(1,1,MakeColor(100,100,100));
    SetColor(1,4,MakeColor(100,0,0));
    SetColor(1,5,MakeColor(100,0,0));
    SetColor(1,6,MakeColor(100,0,0));
    ChangeColorImm();
    sleep(5);

    return 0;
}
/*
         4
face:   3012
         5

lednum: 012
        345
        678
*/
Color Red={255,0,0};
Color Green={0,255,0};
Color Blue={0,0,255};

void ButtonDown(int face, int lednum)
{
    /* ---------- 여기에 몇 번 face의 몇 번 버튼이 눌렸을 때 어떤 동작을 수행할지를 작성 ----------*/

    // 버튼 기준 상하좌우 십자 모양으로 LED 토글하는 애플리케이션
    if(face==0&&lednum==2){
        exit(999);
    }
    int num=rand()%6;
    // tone(3);
    Color now;
    switch (rand()%3)
    {
    case 0:
        now=Red;
        break;
    case 1:
        now=Blue;
        break;
    default:
        now=Green;
        break;
    }
    CubeInit();
    
    switch (num)
    {
    case 0:
        for (int i=0;i<6;++i){
            SetColor(i,4,now);
        }
        break;
    case 1:
        for (int i=0;i<6;++i){
            SetColor(i,8,now);
            SetColor(i,0,now);
        }
        break;
    case 2:
        for (int i=0;i<6;++i){
            SetColor(i,4,now);
            SetColor(i,6,now);
            SetColor(i,2,now);
        }
        break;
    case 3:
        for (int i=0;i<6;++i){
            SetColor(i,0,now);
            SetColor(i,2,now);
            SetColor(i,6,now);
            SetColor(i,8,now);
        }
        break;
    case 4:
        for (int i=0;i<6;++i){
            SetColor(i,0,now);
            SetColor(i,2,now);
            SetColor(i,6,now);
            SetColor(i,8,now);
            SetColor(i,4,now);
        }
        break;
    case 5:
        for (int i=0;i<6;++i){
            SetColor(i,0,now);
            SetColor(i,1,now);
            SetColor(i,2,now);
            SetColor(i,6,now);
            SetColor(i,7,now);
            SetColor(i,8,now);
        }
        break;
    default:
        break;
    }
    ChangeColorImm();
    // 눌린 버튼 색을 무작위로 바꾸는 애플리케이션
    // SetColor(face, lednum, MakeRandomColor());
    // StartFadeColorCoroutine(FadeColor, face, lednum, 5000);

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
    // StartCoroutine(pressButtonsAutomatically);
    srand((unsigned int)time(NULL));
}

// 매 Timing Sequence (deltaTime ms) 마다 실행된다.
void Update()
{
    int a;
}
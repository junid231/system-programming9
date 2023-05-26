#include "coroutine.h"
#include "cube.h"

void ButtonDown(int face, int lednum)
{
    /* ---------- 여기에 몇 번 face의 몇 번 버튼이 눌렸을 때 어떤 동작을 수행할지를 기술 ----------*/

    // 버튼 기준 상하좌우 십자 모양으로 LED 토글하는 애플리케이션
    if(GetColor(face, lednum).r == 0 && GetColor(face, lednum).g == 0 && GetColor(face, lednum).b == 0)
        SetColor(face, lednum, MakeRandomColor());
    else SetColor(face, lednum, MakeColor(0, 0, 0));
    for (int i = 0; i < 4; i++)
    {
        LED *led = GetDirLED(face, lednum, i);
        if(led->color.r == 0 && led->color.g == 0 && led->color.b == 0)
            SetColor(led->face, led->lednum, MakeRandomColor());
        else SetColor(led->face, led->lednum, MakeColor(0, 0, 0));
    }
    StartCoroutine(FadeColor);

    /* ---------------------------------------------------------------------------------------*/
}

void ButtonUp(int face, int lednum)
{
    // if (face == 0 && lednum == 4) prepareMenu = 0;

    /* --------- 여기에 몇 번 face의 몇 번 버튼이 떼어졌을 때 어떤 동작을 수행할지를 기술 ---------*/



    /* ---------------------------------------------------------------------------------------*/
}

void ButtonLongpress(int face, int lednum)
{
    // Goto Menu
    /*
    if(face == 0 && lednum == 4)
    {
        if (!prepareMenu)
        {
            prepareMenu = 1;
            ledCtrl[0, 4].StartTimer();
        }
        else CubeMenu();
    }
    */

    /* ------- 여기에 몇 번 face의 몇 번 버튼이 길게 눌렸을 때 어떤 동작을 수행할지를 기술 --------*/



    /* ---------------------------------------------------------------------------------------*/
}

void pressButtonsAutomatically(Coroutine *coroutine) {
    BEGIN_COROUTINE(coroutine);

    ButtonDown(2, 3);
    WAIT_FOR_SECONDS(coroutine, 5);

    ButtonDown(8, 3);
    WAIT_FOR_SECONDS(coroutine, 5);

    ButtonDown(2, 3);
    WAIT_FOR_SECONDS(coroutine, 5);

    END_COROUTINE();
}

// Called Before the First Update (Just Once)
void Start()
{
    // Cube Color Initiate
    for(int face=0; face<6; face++) {
        for(int lednum=0; lednum<9; lednum++) {
            SetColor(face, lednum, MakeColor(0, 0, 0));
        }
    }
    ChangeColorImm();
    StartCoroutine(pressButtonsAutomatically);
}

// Called For Every Timing Sequence
void Update()
{   
    
    for(int face=0; face<6; face++) {
        for(int lednum=0; lednum<9; lednum++) {
            SetColor(face, lednum, MakeRandomColor());
        }
    }
    
}
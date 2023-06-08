#include "./libcube/coroutine.h"
#include "./libcube/cube.h"

int main() {
    StartCubeRoutine();
    SetColor(1,1,MakeColor(100,100,100));
    return 0;
}

void ButtonDown(int face, int lednum)
{
    /* ---------- 여기에 몇 번 face의 몇 번 버튼이 눌렸을 때 어떤 동작을 수행할지를 작성 ----------*/

    // 버튼 기준 상하좌우 십자 모양으로 LED 토글하는 애플리케이션
    if (face==0&&lednum==0){
        exit(0);
    }
    int newface = face;
    int newlednum = lednum;
    for (int i = -1; i < 4; i++)
    {
        Color color = i != -1 ? GetDirLED(face, lednum, &newface, &newlednum, i) : GetColor(face, lednum);
        if(color.r == 0 && color.g == 0 && color.b == 0)
            SetColor(newface, newlednum, MakeRandomColor());
        else SetColor(newface, newlednum, MakeColor(0, 0, 0));
        StartFadeColorCoroutine(FadeColor, newface, newlednum, 1000);
    }


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
        WAIT_FOR_MILISEC(coroutine, 2000);
    }

    END_COROUTINE(coroutine);
}

// 프로그램이 시작되면 최초 한 번 실행된다.
void Start()
{
    StartCoroutine(pressButtonsAutomatically);
}

// 매 Timing Sequence (deltaTime ms) 마다 실행된다.
void Update()
{
    int a;
}
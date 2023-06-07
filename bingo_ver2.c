#include "./libcube/coroutine.h"
#include "./libcube/cube.h"

int main() {
    StartCubeRoutine();
    return 0;
}

static int current_bingo [6][9] = 
{
    {  0, 0, 0, 0, 0, 0, 0, 0, 0 },
    {  0, 0, 0, 0, 0, 0, 0, 0, 0 },
    {  0, 0, 0, 0, 0, 0, 0, 0, 0 },
    {  0, 0, 0, 0, 0, 0, 0, 0, 0 },
    {  0, 0, 0, 0, 0, 0, 0, 0, 0 },
    {  0, 0, 0, 0, 0, 0, 0, 0, 0 }

};
static int bingo_count = 0;
int bingo_buffer = 0;
bool is_end = false;
void ButtonDown(int face, int lednum)
{
    /* ---------- 여기에 몇 번 face의 몇 번 버튼이 눌렸을 때 어떤 동작을 수행할지를 작성 ----------*/

    // 버튼 기준 상하좌우 십자 모양으로 LED 토글하는 애플리케이션
    
    
    if (bingo_count < 3){
        int newface = face;
        int newlednum = lednum;
        SetColor(face, lednum, MakeColor(0, 0, 255));
        StartFadeColorCoroutine(FadeColor, face, lednum, 4000);
        current_bingo[face][lednum] = 1;
        if (is_end) {
            for(int i=0; i<6; i++){
                for(int j=0; j<9; j++){
                    if (current_bingo[i][j] == 1){
                        current_bingo[i][j] = 0;
                        SetColor(i, j, MakeColor(0, 0, 0));
                        StartFadeColorCoroutine(FadeColor, i, j, 4000);
                    }
                }
            }
            is_end = false;
        }
        if(GetDirLED(face, lednum, &newface, &newlednum, 0).b == 255 && GetDirLED(face, lednum, &newface, &newlednum, 2).b == 255){
            bingo_count++;
        }
        if(GetDirLED(face, lednum, &newface, &newlednum, 1).b == 255 && GetDirLED(face, lednum, &newface, &newlednum, 3).b == 255){
            bingo_count++;
        }
        if(GetDirLED(face, lednum, &newface, &newlednum, 0).b == 255 && GetDirLEDwithDist(face, lednum, &newface, &newlednum, 0, 2).b == 255){
            bingo_count++;
        }
        if(GetDirLED(face, lednum, &newface, &newlednum, 1).b == 255 && GetDirLEDwithDist(face, lednum, &newface, &newlednum, 1, 2).b == 255){
            bingo_count++;
        }
        if(GetDirLED(face, lednum, &newface, &newlednum, 2).b == 255 && GetDirLEDwithDist(face, lednum, &newface, &newlednum, 2, 2).b == 255){
            
            bingo_count++;
        }
        if(GetDirLED(face, lednum, &newface, &newlednum, 3).b == 255 && GetDirLEDwithDist(face, lednum, &newface, &newlednum, 3, 2).b == 255){
            printf("inin\n");
            bingo_count++;
        }
        bingo_buffer = bingo_count;
        if(lednum == 4){
            if (GetColor(face, 0).b == 255 && GetColor(face, 8).b == 255){
                bingo_count++;
                newface = face;
                newlednum = lednum;
            } 
            if (GetColor(face, 2).b == 255 && GetColor(face, 6).b == 255){
                bingo_count++;
                newface = face;
                newlednum = lednum;
            }
            return;
        } else if(lednum == 0){
            if (GetColor(face, 4).b == 255 && GetColor(face, 8).b == 255){
                bingo_count++;
                newface = face;
                newlednum = lednum;
            }
            return;
        } else if(lednum == 2){
            if (GetColor(face, 4).b == 255 && GetColor(face, 6).b == 255){
                bingo_count++;
                newface = face;
                newlednum = lednum;
            }
            return;
        } else if(lednum == 6){
            if (GetColor(face, 4).b == 255 && GetColor(face, 2).b == 255){
                bingo_count++;
                newface = face;
                newlednum = lednum;
            }
            return;
        } else if(lednum == 8){
            if (GetColor(face, 4).b == 255 && GetColor(face, 0).b == 255){
                bingo_count++;
                newface = face;
                newlednum = lednum;
            }
            return;
        }
    } 
    

    // 눌린 버튼 색을 무작위로 바꾸는 애플리케이션
    // SetColor(face, lednum, MakeRandomColor());
    // StartFadeColorCoroutine(FadeColor, face, lednum, 5000);

    /* ---------------------------------------------------------------------------------------*/
}

void ButtonUp(int face, int lednum)
{
    /* --------- 여기에 몇 번 face의 몇 번 버튼이 떼어졌을 때 어떤 동작을 수행할지를 작성 ---------*/
    while (bingo_count >= 3) {
        for(int i=0; i<6; i++){
            for(int j=0; j<9; j++){
                if (current_bingo[i][j] == 1){
                    SetColor(i, j, MakeColor(255, 0, 0));
                    StartFadeColorCoroutine(FadeColor, i, j, 4000);
                }
            }
        }
        bingo_count = 0;
        is_end = true;
        printf("inin\n");
        break;
    
    }

    /* ---------------------------------------------------------------------------------------*/
}

// Button Input을 여기에 구현
void pressButtonsAutomatically(Coroutine *coroutine) {
    BEGIN_COROUTINE(coroutine);
    ButtonDown(0, 4);
    ButtonUp(0, 4);
    WAIT_FOR_MILISEC(coroutine, 4000);
    ButtonDown(0, 2);
    ButtonUp(0, 2);
    WAIT_FOR_MILISEC(coroutine, 4000);
    ButtonDown(1, 0);
    ButtonUp(1, 0);
    WAIT_FOR_MILISEC(coroutine, 4000);
    ButtonDown(0, 1);
    ButtonUp(0, 1);
    WAIT_FOR_MILISEC(coroutine, 4000);
    ButtonDown(0, 3);
    ButtonUp(0, 3);
    WAIT_FOR_MILISEC(coroutine, 4000);
    ButtonDown(0, 5);
    ButtonUp(0, 5);
    WAIT_FOR_MILISEC(coroutine, 4000);
    ButtonDown(0, 7);
    ButtonUp(0, 7);
    WAIT_FOR_MILISEC(coroutine, 4000);
    ButtonDown(2, 1);
    
    
    // 마구잡이로 아무 버튼이나 클릭하는 무한루프...

    

    

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
    printf("%d   ", bingo_count);
    printf("%d", bingo_buffer);
}
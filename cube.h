typedef struct {
    int r;
    int g;
    int b;
} Color;

typedef struct {
    int face;
    int lednum;
    Color color;
} LED;

/*
        4
face:   0123
        5

lednum: 012
        345
        678
*/

/* ========================== cubefunc.c ========================== */

// 특정 위치의 led의 color를 받는다.
Color GetColor(int face, int lednum);
// 특정 위치의 led의 color를 설정한다.
void SetColor(int face, int lednum, Color newcolor);

// 특정 위치 기준 특정 방향에 있는 LED 정보를 가져온다.
// dir이 0, 1, 2, 3일 때 순서대로 위, 오른쪽, 아래, 왼쪽을 나타낸다.
LED *GetDirLED(int face, int lednum, int dir);
// 특정 위치 기준 특정 방향으로 dist만큼 떨어진 LED 정보를 가져온다.
LED *GetDirLEDwithDist(int face, int lednum, int dir, int dist);
// 특정 위치의 반대편에 있는 LED 정보를 가져온다.
LED *GetOppositeLED(int face, int lednum);

Color MakeRandomColor();
Color MakeColor(int r, int g, int b);

// 코루틴 함수: StartCoroutine(FadeColor) 로 호출
// 코루틴 호출한 시점에서 비동기적으로 Fade 처리된다.
void FadeColor(Coroutine* coroutine);

/* ========================== cubeapp.c ========================== */

void ButtonDown(int face, int lednum);
void ButtonUp(int face, int lednum);
void ButtonLongpress(int face, int lednum);

/*
         4
face:   3012
         5

lednum: 012
        345
        678
*/

/* =========================== cubeio.c =========================== */

void tone(Coroutine *coroutine);
void StartCubeRoutine();

/* ========================== cubefunc.c ========================== */

// 특정 위치의 LED의 color를 받는다.
Color GetColor(int face, int lednum);
// 특정 위치의 LED의 color를 설정한다.
void SetColor(int face, int lednum, Color newcolor);
// r, g, b값에 해당하는 Color 구조체 변수를 만든다.
Color MakeColor(int r, int g, int b);
// 무작위 색깔이 배정된 Color 구조체 변수를 만든다.
Color MakeRandomColor();

// 특정 위치 기준 특정 방향에 있는 LED 정보를 가져온다.
// dir이 0, 1, 2, 3일 때 순서대로 위, 오른쪽, 아래, 왼쪽을 나타낸다.
Color GetDirLED(int face, int lednum, int *newface, int *newlednum, int dir);
// 특정 위치 기준 특정 방향으로 dist만큼 떨어진 LED 정보를 가져온다.
Color GetDirLEDwithDist(int face, int lednum, int *newface, int *newlednum, int dir, int dist);
// 특정 위치의 반대편에 있는 LED 정보를 가져온다.
Color GetOppositeLED(int face, int lednum, int *newface, int *newlednum);

// 방법 1: SetColor로 변경한 LED 색을 즉시 반영한다.
void ChangeColorImm();
// 방법 2: SetColor로 변경한 LED 색을 비동기적으로 Fade 처리한다.
// 코루틴 함수 호출방법: StartFadeColorCoroutine(FadeColor, newface, newlednum, durationms);
void FadeColor(Coroutine* coroutine);

// 큐브를 초기화한다. (모두 Color(0,0,0)으로 즉시 설정)
void CubeInit();
// Fade 처리중인 실제 LED color를 받는다. (Application 구현에는 사용하지 않음)
Color GetCurrColor(int face, int lednum);

/* ========================== main.c에서 구현해야 하는 함수 ========================== */

// LED 버튼이 눌렸을 때 실행된다.
void ButtonDown(int face, int lednum);
// LED 버튼이 떼어졌을 때 실행된다.
void ButtonUp(int face, int lednum);

// 프로그램이 시작되면 최초 한 번 실행된다.
void Start();
// 매 Timing Sequence (deltaTime ms) 마다 실행된다.
void Update();
# DLC LED Cube API
### System programming project team-9

----

#### 프로젝트 설명

libcube 폴더에 Dynamic Linking Library로 다양한 함수를 구현하였습니다.

cube.h, coroutine.h에 설명이 정리되어 있습니다.

#### Coroutine이란?

Coroutine(코루틴)은 프로그램의 실행을 제어하기 위해 사용되는 개념입니다. 코루틴은 일시 중지하고 다시 시작할 수 있는 함수입니다. 코루틴을 사용하면 코드 실행을 잠시 중단하고 다른 작업을 수행한 후, 나중에 다시 해당 지점에서 실행을 재개할 수 있습니다.

코루틴은 주로 게임 개발에서 사용되며, 다음과 같은 목적으로 활용될 수 있습니다:

복잡한 애니메이션 및 이펙트 시퀀스 관리
네트워크 통신 작업
AI 동작 및 상호 작용
대규모 데이터 처리를 작은 단위로 분할하여 진행
시간 지연 기능을 가진 반복 작업 수행

#### Coroutine 함수 직접 구현 방법

Coroutine 함수는 다음과 같이 정의합니다.

    void CoroutineFunctionName(Coroutine *coroutine);

Coroutine 함수는 기본적으로 아래와 같이 구현합니다.

    BEGIN_COROUTINE(coroutine);
    /* Code Block */
    END_COROUTINE(coroutine);

Code Block 내부에 YIELD 및 WAIT_FOR_MILISEC 문을 활용해 일시 중지하고 나중에 다시 돌아오도록 할 수 있습니다.

YIELD문은 한 Frame (deltaTime milisecond) 후에 바로 다시 돌아와 실행하도록 하고, WAIT_FOR_MILISEC 문은 설정한 ms 후에 다시 돌아와 실행하도록 합니다.

    YIELD(coroutine);
    WAIT_FOR_MILISEC(coroutine, ms);
    
#### Coroutine 함수 실행 방법

Coroutine 함수는 다음과 같은 방법으로 실행합니다.

    StartCoroutine(CoroutineFunctionName);

Fade Coroutine 함수는 예외적으로 다음과 같은 방법으로 실행합니다.

    StartFadeColorCoroutine(FadeColor, face, lednum, fadems);

Coroutine 함수는 다음과 같은 방법으로 중지합니다.

    StopCoroutine(CoroutineFunctionName);

#### 프로젝트 요구사항

main.c의 상위 디렉토리에 libcube가 있어야 합니다.

    #include "./libcube/coroutine.h"
    #include "./libcube/cube.h"

main.c에서 아래 함수들을 구현하여야 합니다.
이를 구현함으로서 cube application 및 interface 구현을 customize할 수 있습니다.

    // LED 버튼이 눌렸을 때 실행된다.
    void ButtonDown(int face, int lednum);
    // LED 버튼이 떼어졌을 때 실행된다.
    void ButtonUp(int face, int lednum);
    // 프로그램이 시작되면 최초 한 번 실행된다.
    void Start();
    // 매 Timing Sequence (deltaTime ms) 마다 실행된다.
    void Update();

#### 빌드 방법

main.c와 cube library를 linking하여 build하기 위해서는 아래의 명령어를 사용하면 됩니다. 

    ./build.sh

빌드 후에 runcube 실행파일이 만들어지면 아래의 명령어를 사용하여 실행하면 됩니다.

    ./runcube
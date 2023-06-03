#include <stdio.h>
#include <unistd.h>
#include "coroutine.h"
#include "cube.h"

#include <wiringPi.h>
#include <wiringPiSPI.h>

#define PL    2             // chip 1 : Digital out, LOW일때 들어오는 스위치 값 받음. 
#define CP    11            // chip 2 : SPI CLK (SCK) : SCLK, 클럭용 핀
#define CHANNEL     9       // chip 9 : SPI MISO : 값 읽어들이는 핀
#define NUM_INPUTS  2       // 74HC165 모듈의 입력 개수
#define LED_PIN_START   3   // LED 0번 핀

static bool buttonState[6*9];
static bool buttonprevState[6*9];

// Implement Terminal Output
void TerminalOutput() {
    system("clear");
    printf("\n");
    int face = 4;
    for(int lednum=0; lednum<9; lednum++) {
        if(lednum % 3 == 0) printf("    ");
        Color ledcolor = GetCurrColor(face, lednum);
        // Set text color using RGB values
        printf("\033[38;2;%d;%d;%dm%s\033[0m", 
            ledcolor.r, ledcolor.g, ledcolor.b, "■");
        if(lednum % 3 == 2) printf("\n");
    }
    face = 3;
    int lednum = 0;
    while(lednum < 9) {
        Color ledcolor = GetCurrColor(face, lednum);
        // Set text color using RGB values
        printf("\033[38;2;%d;%d;%dm%s\033[0m", 
            ledcolor.r, ledcolor.g, ledcolor.b, "■");
        if(lednum % 3 == 2) {
            if(face == 2) {
                printf("\n");
                lednum++;
                face = 3;
            }
            else {
                printf(" ");
                lednum -= 2;
                face = (face + 1) % 4;
            }
        }
        else lednum++;
    }
    face = 5;
    for(int lednum=0; lednum<9; lednum++) {
        if(lednum % 3 == 0) printf("    ");
        Color ledcolor = GetCurrColor(face, lednum);
        // Set text color using RGB values
        printf("\033[38;2;%d;%d;%dm%s\033[0m", 
            ledcolor.r, ledcolor.g, ledcolor.b, "■");
        if(lednum % 3 == 2) printf("\n");
    }
}

void StartCubeRoutine() {
    InitCoroutines();
    CubeInit();
    srand(time(NULL));
    memset(buttonprevState, 0, sizeof(bool) * 6*9);

    int fd = wiringPiSPISetup(CHANNEL, 500000);
    unsigned char inputData = 0;

    // LED 핀을 출력으로 설정
    for(int pin=0; pin<NUM_INPUTS; pin++)
        pinMode(pin+LED_PIN_START, OUTPUT);

    Start();

    while (true) {
        CallCoroutines();

        // Real Hardware Version
        for(int pin=0; pin<NUM_INPUTS; pin++) {
            wiringPiSPIDataRW(CHANNEL, &inputData, sizeof(inputData));
            if (inputData & (1 << pin)) {
                digitalWrite(pin+LED_PIN_START, HIGH);
                printf("a");
            } else {
                digitalWrite(pin+LED_PIN_START, LOW);
                printf("b");
            }
        }

        // Terminal Version
        // TerminalOutput();

        /* ==================================================== */

        Update();
        usleep(deltaTime*1000);
    }
}
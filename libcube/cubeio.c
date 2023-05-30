#include <stdio.h>
#include <unistd.h>
#include "coroutine.h"
#include "cube.h"

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
    Start();

    while (true) {
        CallCoroutines();

        /* =============== Button Input Control =============== */

        // Real Hardware Version
        /*for(int face=0; face<6; face++) {
            for(int lednum=0; lednum<9; lednum++) {
                if(buttonInput(face*lednum) && !buttonprevState[face*lednum])
                    ButtonDown(face, lednum);
                if(!buttonInput(face*lednum) && buttonprevState[face*lednum])
                    ButtonUp(face, lednum);
            }
        }*/

        // Terminal Version
        // TerminalInput();

        /* ================ LED Output Control ================ */

        // Real Hardware Version

        // Terminal Version
        TerminalOutput();

        /* ==================================================== */

        Update();
        usleep(deltaTime*1000);
        // sleep((float)deltaTime/1000);
    }
}
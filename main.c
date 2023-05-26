#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "coroutine.h"
#include "cube.h"

extern CoroutineHandle coroutineHandles[MAX_COROUTINES];
bool buttonState[6*9];
bool buttonprevState[6*9];

// Implement Terminal Output
void TerminalOutput() {
    system("clear");
    int face = 4;
    for(int lednum=0; lednum<9; lednum++) {
        LED *led = GetDirLED(face, lednum, 4);
        // Set text color using RGB values
        printf("\033[38;2;%d;%d;%dm%s\033[0m", 
            led->color.r, led->color.g, led->color.b, "■");
        if(lednum % 3 == 2) printf("\n");
    }
    printf("\n");
    face = 0;
    int lednum = 0;
    while(lednum < 9) {
        LED *led = GetDirLED(face, lednum, 4);
        // Set text color using RGB values
        printf("\033[38;2;%d;%d;%dm%s\033[0m", 
            led->color.r, led->color.g, led->color.b, "■");
        if(lednum % 3 == 2) {
            if(face == 3) {
                printf("\n");
                lednum++;
                face = 0;
            }
            else {
                printf(" ");
                lednum -= 2;
                face++;
            }
        }
        else lednum++;
    }
    printf("\n");
    face = 5;
    for(int lednum=0; lednum<9; lednum++) {
        LED *led = GetDirLED(face, lednum, 4);
        // Set text color using RGB values
        printf("\033[38;2;%d;%d;%dm%s\033[0m", 
            led->color.r, led->color.g, led->color.b, "■");
        if(lednum % 3 == 2) printf("\n");
    }
}

int main() {
    int i;
    for (i = 0; i < MAX_COROUTINES; i++)
        coroutineHandles[i].coroutine.isRunning = false;
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
        usleep(deltaTime*100000);
    }

    return 0;
}
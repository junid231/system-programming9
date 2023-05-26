#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "coroutine.h"
#include "cube.h"

void Start();
void Update();

extern CoroutineHandle coroutineHandles[MAX_COROUTINES];
bool buttonState[6*9];
bool buttonprevState[6*9];

int main() {
    int i;
    for (i = 0; i < MAX_COROUTINES; i++)
        coroutineHandles[i].coroutine.isRunning = false;
    srand(time(NULL));
    Start();

    while (true) {
        CallCoroutines();
        Update();
        sleep(deltaTime);
    }

    return 0;
}

// Implement Terminal Input
void TerminalInput() {
    // socket 통신 방식...?
}

// Implement Terminal Output
void TerminalOutput() {
    system("clear");
    int face = 4;
    for(int lednum=0; lednum<9; lednum++) {
        LED *led = GetDirLED(face, lednum, 4);
        // Set text color using RGB values
        printf("\033[38;2;%d;%d;%dm%c\033[0m", 
            led->color.r, led->color.g, led->color.b, 254);
        if(lednum % 3 == 2) printf("\n");
    }
    printf("\n");
    face = 0;
    int lednum = 0;
    while(lednum < 9) {
        LED *led = GetDirLED(face, lednum, 4);
        // Set text color using RGB values
        printf("\033[38;2;%d;%d;%dm%c\033[0m", 
            led->color.r, led->color.g, led->color.b, 254);
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
        printf("\033[38;2;%d;%d;%dm%c\033[0m", 
            led->color.r, led->color.g, led->color.b, 254);
        if(lednum % 3 == 2) printf("\n");
    }
}

// Called Before the First Update (Just Once)
void Start()
{
    memset(buttonprevState, 0, sizeof(bool) * 6*9);
}

// Called For Every Timing Sequence
void Update()
{
    /*
    // Real Hardware Version
    for(int face=0; face<6; face++) {
        for(int lednum=0; lednum<9; lednum++) {
            if(buttonInput(face*lednum) && !buttonprevState[face*lednum])
                ButtonDown(face, lednum);
            if(!buttonInput(face*lednum) && buttonprevState[face*lednum])
                ButtonUp(face, lednum);
        }
    }
    */

    // Terminal Version
    TerminalInput();
    TerminalOutput();
}
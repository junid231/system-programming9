#include <stdio.h>
#include "coroutine.h"

CoroutineHandle coroutineHandles[MAX_COROUTINES];

void StartCoroutine(CoroutineFunction function) {
    int i;
    for (i = 0; i < MAX_COROUTINES; i++) {
        if (!coroutineHandles[i].coroutine.isRunning) {
            coroutineHandles[i].function = function;
            coroutineHandles[i].coroutine.isRunning = true;
            coroutineHandles[i].coroutine.currentState = 0;
            return;
        }
    }

    printf("Max coroutines reached, unable to start a new coroutine.\n");
}

void CallCoroutines() {
    int i;
    for (i = 0; i < MAX_COROUTINES; i++) {
        CoroutineHandle* handle = &coroutineHandles[i];
        if (handle->coroutine.isRunning) {
            handle->function(&handle->coroutine);
        }
    }
}
#include "coroutine.h"

static CoroutineHandle coroutineHandles[MAX_COROUTINES];

void InitCoroutines() {
    for (int i = 0; i < MAX_COROUTINES; i++)
        coroutineHandles[i].coroutine.isRunning = false;
}

void StartCoroutine(CoroutineFunction function) {
    for (int i = 0; i < MAX_COROUTINES; i++) {
        if (!coroutineHandles[i].coroutine.isRunning) {
            coroutineHandles[i].function = function;
            coroutineHandles[i].coroutine.isRunning = true;
            coroutineHandles[i].coroutine.currentState = 0;
            return;
        }
    }
}

void StartFadeColorCoroutine(CoroutineFunction function, int face, int lednum, int durationms) {
    for (int i = 0; i < MAX_COROUTINES; i++) {
        // if same led was in process -> stop it
        if(coroutineHandles[i].function == function
        && coroutineHandles[i].coroutine.face == face
        && coroutineHandles[i].coroutine.lednum == lednum)
            coroutineHandles[i].coroutine.isRunning = false;

        if (!coroutineHandles[i].coroutine.isRunning) {
            coroutineHandles[i].function = function;
            coroutineHandles[i].coroutine.isRunning = true;
            coroutineHandles[i].coroutine.currentState = 0;
            coroutineHandles[i].coroutine.face = face;
            coroutineHandles[i].coroutine.lednum = lednum;
            coroutineHandles[i].coroutine.durationms = durationms;
            return;
        }
    }
}

void StopCoroutine(CoroutineFunction function) {
    for (int i = 0; i < MAX_COROUTINES; i++) {
        if (coroutineHandles[i].function == function) {
            coroutineHandles[i].coroutine.isRunning = false;
            return;
        }
    }
}

void CallCoroutines() {
    for (int i = 0; i < MAX_COROUTINES; i++) {
        CoroutineHandle* handle = &coroutineHandles[i];
        if (handle->coroutine.isRunning) {
            handle->function(&handle->coroutine);
        }
    }
}
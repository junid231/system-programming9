#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX_COROUTINES 60
#define deltaTime 1000       // 1 cycle == deltaTime ms

typedef struct {
    int r;
    int g;
    int b;
} Color;

typedef struct {
    bool isRunning;
    int currentState;
    // WAIT_FOR_MILISEC Parameters
    int elapsedms;
    int waitms;
    // FadeColor Parameters
    int face;
    int lednum;
    int durationms;
    int processms;
    Color startcolor;
} Coroutine;

typedef void (*CoroutineFunction)(Coroutine*);

typedef struct {
    CoroutineFunction function;
    Coroutine coroutine;
} CoroutineHandle;

#define BEGIN_COROUTINE(coroutine) \
    switch (coroutine->currentState) { \
    case 0:

#define YIELD(coroutine) \
        do { \
            coroutine->currentState = __LINE__; \
            return; \
    case __LINE__:; \
        } while (0);

#define WAIT_FOR_MILISEC(coroutine, ms) \
        do { \
            coroutine->currentState = __LINE__; \
            coroutine->elapsedms = 0; \
            coroutine->waitms = ms; \
            return; \
    case __LINE__:; \
            if (coroutine->elapsedms < coroutine->waitms) { \
                coroutine->elapsedms += deltaTime; \
                return; \
            } \
        } while (0);
    
#define END_COROUTINE(coroutine) \
        coroutine->isRunning = false; \
    }

// Init All Coroutines to zero state
void InitCoroutines();
// Starting Coroutine Generally
void StartCoroutine(CoroutineFunction function);
// Starting Coroutine when FadeColor
void StartFadeColorCoroutine(CoroutineFunction function, int face, int lednum, int durationms);

void StopCoroutine(CoroutineFunction function);
void CallCoroutines();
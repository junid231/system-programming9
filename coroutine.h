#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define MAX_COROUTINES 10
#define deltaTime 1.0f     // 1 frame == 50 ms

typedef struct {
    bool isRunning;
    int currentState;
    clock_t startTime;
    double waitTime;
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

#define WAIT_FOR_SECONDS(coroutine, seconds) \
    do { \
        coroutine->currentState = __LINE__; \
        coroutine->startTime = clock(); \
        coroutine->waitTime = (seconds) * CLOCKS_PER_SEC; \
        return; \
        case __LINE__:; \
        if (clock() - coroutine->startTime < coroutine->waitTime) \
            return; \
    } while (0);
    
#define END_COROUTINE() \
    }

void StartCoroutine(CoroutineFunction function);
void CallCoroutines();
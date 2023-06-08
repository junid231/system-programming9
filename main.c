#include "./libcube/coroutine.h"
#include "./libcube/cube.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

static int state[6][9];
static int isGaming = 0;

static Color blinkcolor;
static int win_num = 0;
static int lose_num = 0;
static int blinkface = -1;
static int blinklednum = -1;

void tongshin(Coroutine *coroutine) {
    static struct addrinfo hints, *res;
    static int sockfd, clientfd, status;
    static char buffer[1024];

    BEGIN_COROUTINE(coroutine);

    // Set up socket parameters
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    
    // Resolve the IP address of the local host using getaddrinfo
    status = getaddrinfo(NULL, "5000", &hints, &res);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }

    // Create a socket object and bind it to the local address
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        perror("socket error");
        exit(1);
    }

    static int reuse = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof reuse);

    status = bind(sockfd, res->ai_addr, res->ai_addrlen);
    if (status == -1) {
        perror("bind error");
        exit(1);
    }

    // Listen for incoming connections
    status = listen(sockfd, 1);
    if (status == -1) {
        perror("listen error");
        exit(1);
    }
    
    // printf("Listening on port 5000...\n");

    while (1) {
        // Accept a new connection
        clientfd = accept(sockfd, NULL, NULL);
        if (clientfd == -1) {
            perror("accept error");
            continue;
        }
        
        // printf("Accepted connection from client\n");

        // Receive the message from the client
        int bytes_received = recv(clientfd, buffer, sizeof buffer - 1, 0);
        buffer[bytes_received] = '\0';
        // printf("/%s/", buffer);

        int face, lednum;
        sscanf(buffer, "%d %d", &face, &lednum);
        ButtonDown(face, lednum);
        WAIT_FOR_MILISEC(coroutine, 2000);

        // Send a response back to the client
        const char *response = "Successfully Recieved!!\n";
        send(clientfd, response, strlen(response), 0);
        // printf("Sent response to client\n");

        // Close the socket connection
        close(clientfd);
        // printf("Closed connection to client\n");
        YIELD(coroutine);
    }

    // Free the address information
    freeaddrinfo(res);
    END_COROUTINE(coroutine);
}

void Blink(Coroutine *coroutine) {
    BEGIN_COROUTINE(coroutine);

    SetColor(blinkface, blinklednum, blinkcolor);
    StartFadeColorCoroutine(FadeColor, blinkface, blinklednum, 500);

    WAIT_FOR_MILISEC(coroutine, 1000);

    state[blinkface][blinklednum] = 0;
    SetColor(blinkface, blinklednum, MakeColor(0, 0, 0));
    StartFadeColorCoroutine(FadeColor, blinkface, blinklednum, 1000);

    END_COROUTINE(coroutine);
}

int main() {
    StartCubeRoutine();
    return 0;
}

void ButtonDown(int face, int lednum)
{
    /* ---------- 여기에 몇 번 face의 몇 번 버튼이 눌렸을 때 어떤 동작을 수행할지를 작성 ----------*/

    // 버튼 기준 상하좌우 십자 모양으로 LED 토글하는 애플리케이션
    // int newface = face;
    // int newlednum = lednum;
    // for (int i = -1; i < 4; i++)
    // {
    //     Color color = i != -1 ? GetDirLED(face, lednum, &newface, &newlednum, i) : GetColor(face, lednum);
    //     if(color.r == 0 && color.g == 0 && color.b == 0)
    //         SetColor(newface, newlednum, MakeRandomColor());
    //     else SetColor(newface, newlednum, MakeColor(0, 0, 0));
    //     StartFadeColorCoroutine(FadeColor, newface, newlednum, 1000);
    // }

    // 눌린 버튼 색을 무작위로 바꾸는 애플리케이션
    Color color;
    int choice = rand() % 3;
    color.r = choice == 0 ? 255 : 0;
    color.g = choice == 1 ? 255 : 0;
    color.b = choice == 2 ? 255 : 0;
    SetColor(face, lednum, color);
    StartFadeColorCoroutine(FadeColor, face, lednum, 1000);

    // 두더지잡기
    if(isGaming) {
        if(state[face][lednum]) {
            blinkcolor = MakeColor(0, 0, 255);
            blinkface = face;
            blinklednum = lednum;
            StartCoroutine(Blink);
            state[face][lednum] = 0;
            win_num++;
        }
        else {
            blinkcolor = MakeColor(255, 0, 0);
            blinkface = face;
            blinklednum = lednum;
            StartCoroutine(Blink);
            state[face][lednum] = 0;
            lose_num++;
        }
    }
    
    /* ---------------------------------------------------------------------------------------*/
}

void ButtonUp(int face, int lednum)
{
    /* --------- 여기에 몇 번 face의 몇 번 버튼이 떼어졌을 때 어떤 동작을 수행할지를 작성 ---------*/

    

    /* ---------------------------------------------------------------------------------------*/
}

void startGame(Coroutine *coroutine) {
    static time_t startTime;

    BEGIN_COROUTINE(coroutine);
    isGaming = 1;
    win_num = 0;
    lose_num = 0;
    startTime = time(NULL);
    memset(state, 0, sizeof(int)*54);

    while(time(NULL) - startTime < 30) {    // 게임시간 30초
        int face = rand() % 2;
        int lednum = rand() % 8;

        blinkcolor = MakeColor(0, 255, 0);
        blinkface = face;
        blinklednum = lednum;
        StartCoroutine(Blink);
        state[face][lednum] = 1;
        WAIT_FOR_MILISEC(coroutine, 2000);
    }

    for(int i=0; i<18; i++) {
        SetColor(i / 9, i % 9, MakeColor(0, 255, 0));
        StartFadeColorCoroutine(FadeColor, i / 9, i % 9, 1000);
    }
    WAIT_FOR_MILISEC(coroutine, 3000);

    for(int i=0; i<18; i++) {
        if(win_num) {
            win_num--;
            SetColor(i / 9, i % 9, MakeColor(0, 0, 255));
            StartFadeColorCoroutine(FadeColor, i / 9, i % 9, 1000);
            WAIT_FOR_MILISEC(coroutine, 500);
            continue;
        }
        SetColor(i / 9, i % 9, MakeColor(255, 0, 0));
        StartFadeColorCoroutine(FadeColor, i / 9, i % 9, 1000);
        WAIT_FOR_MILISEC(coroutine, 500);
        lose_num--;
    }

    isGaming = 0;
    END_COROUTINE(coroutine);
}

// Button Input을 여기에 구현
void pressButtonsAutomatically(Coroutine *coroutine) {
    BEGIN_COROUTINE(coroutine);

    // 마구잡이로 아무 버튼이나 클릭하는 무한루프...
    while (1) {
        ButtonDown(rand() % 6, rand() % 9);
        WAIT_FOR_MILISEC(coroutine, 2000);
    }

    // static int i;
    // for(i=0; i<54; i++) {
    //     ButtonDown(i / 9, i % 9);
    //     WAIT_FOR_MILISEC(coroutine, 1000);
    // }

    END_COROUTINE(coroutine);
}

// 프로그램이 시작되면 최초 한 번 실행된다.
void Start()
{
    // StartCoroutine(pressButtonsAutomatically);
    StartCoroutine(startGame);
    // StartCoroutine(tongshin);
}

// 매 Timing Sequence (deltaTime ms) 마다 실행된다.
void Update()
{
    
}
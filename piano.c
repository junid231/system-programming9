#include "./libcube/coroutine.h"
#include "./libcube/cube.h"

#include <time.h>

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

//for get input
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

#include <signal.h>

#include <stdlib.h>

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>


static void* ptr;
static const char* name = "input2";
static const int SIZE = sizeof(int)*10;
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
void int_handler(int sig)
{
    int buf[6]={0,0,0,0,0,0};
    printf("Process %d received signal %d\n", getpid(), sig);
    memcpy(buf,ptr,sizeof(int)*4);
    ButtonDown(buf[1],buf[2]);
    printf("button pushed %d %d",buf[1],buf[2]);
    buf[0]=getpid();
    memcpy(ptr,buf,sizeof(int)*4);
}
void exit_handler(int sig){
    munmap(ptr,SIZE);
    exit(0);
}
void getinput(){
    int buf[6]={0,0,0,0,0,0};
    buf[0]=getpid();
    signal(SIGUSR1,int_handler);
    signal(SIGINT,exit_handler);
    /* the size (in bytes) of shared memory object */
    
    /* name of the shared memory object */
    int shm_fd;
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (shm_fd<0){
        printf("faaaail\n");
        exit(1);
    }
    //init the memory
    ftruncate(shm_fd, SIZE);
    /* memory map the shared memory object */
    ptr = mmap(0, SIZE,  PROT_READ|PROT_WRITE,MAP_SHARED, shm_fd, 0);
    memcpy(ptr,buf,sizeof(int)*5);


    
}

int main() {
        // printf("dsfasf\n\n\asdfdsaf\na\sdfasdsfsadf\nsdafnasd\n\n");
        // sleep(3);
    StartCubeRoutine();

    return 0;
}
/*
         4
face:   3012
         5

lednum: 012
        345
        678
*/
Color Red={255,0,0};
Color Green={0,255,0};
Color Blue={0,0,255};

void ButtonDown(int face, int lednum)
{
    /* ---------- 여기에 몇 번 face의 몇 번 버튼이 눌렸을 때 어떤 동작을 수행할지를 작성 ----------*/

    // 버튼 기준 상하좌우 십자 모양으로 LED 토글하는 애플리케이션
    if(face==0&&lednum==0){
        shm_unlink(name);
        exit(0);
    }
    SetColor(face, lednum, MakeRandomColor());
    StartFadeColorCoroutine(FadeColor, face, lednum, 3000);
    tone(lednum);
    usleep(300000);
    tone(0);
    /* ---------------------------------------------------------------------------------------*/
}

void ButtonUp(int face, int lednum)
{
    /* --------- 여기에 몇 번 face의 몇 번 버튼이 떼어졌을 때 어떤 동작을 수행할지를 작성 ---------*/

    

    /* ---------------------------------------------------------------------------------------*/
}

// Button Input을 여기에 구현
void pressButtonsAutomatically(Coroutine *coroutine) {
    BEGIN_COROUTINE(coroutine);

    // 마구잡이로 아무 버튼이나 클릭하는 무한루프...
    while (1) {
        ButtonDown(rand() % 6, rand() % 9);
        WAIT_FOR_MILISEC(coroutine, 1000);
    }

    END_COROUTINE(coroutine);
}

// 프로그램이 시작되면 최초 한 번 실행된다.
void Start()
{
    // StartCoroutine(pressButtonsAutomatically);
    srand((unsigned int)time(NULL));
    // StartCoroutine(tongshin);
    getinput();
}

// 매 Timing Sequence (deltaTime ms) 마다 실행된다.
void Update()
{
    int a;
}
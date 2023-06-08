#include "./libcube/coroutine.h"
#include "./libcube/cube.h"

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

static void* ptr;
static const char* name = "input";
static const int SIZE = sizeof(int)*10;

static char dlcs[52][32];
void int_handler(int sig)
{
    int buf[6]={0,0,0,0,0,0};
    // printf("Process %d received signal %d\n", getpid(), sig);
    memcpy(buf,ptr,sizeof(int)*4);
    ButtonDown(buf[1],buf[2]);
    printf("button pushed %d %d",buf[1],buf[2]);
    buf[0]=getpid();
    memcpy(ptr,buf,sizeof(int)*4);
}
void exit_handler(int sig){
    static int time=0;
    munmap(ptr,SIZE);
    CubeInit();
    ChangeColorImm();
    // WAIT_FOR_MILISEC(coroutine, 1000);
    ++time;
    if (time>1){
        exit(0);
    }
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
    //init the memory
    ftruncate(shm_fd, SIZE);
    /* memory map the shared memory object */
    ptr = mmap(0, SIZE,  PROT_READ|PROT_WRITE,MAP_SHARED, shm_fd, 0);
    memcpy(ptr,buf,sizeof(int)*5);


    //// memcpy(ptr,buf,sizeof(int)*2);

    //// munmap(ptr,SIZE);
    // //ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    
}



int main() {
    
    
    // usleep(200);
    // tone(3);
    // usleep(200);
    // tone(5);
    // usleep(200);
    // tone(8);
    // usleep(200);
    // tone(0);
    StartCubeRoutine();
    SetColor(1,1,MakeColor(100,100,100));

    return 0;
}
int mode=1;
void ButtonDown(int face, int lednum)
{
    printf("%d %s \n", face*9+lednum,dlcs[face*9+lednum]);
    SetColor(face, lednum, MakeRandomColor());
    StartFadeColorCoroutine(FadeColor, face, lednum, 3000);
    if (mode==0){
        shm_unlink(name);
        exit(0);
    }
    // SetColor(0, 2, MakeRandomColor());
    // ChangeColorImm();
    if(face==1 &&lednum==4){
        CubeInit();
        ChangeColorImm();
        mode=0;
        return;
        // exit(333);
    }
    pid_t pid=0;
    if(face*9+lednum<11){
        pid =fork();
        printf("forked\n");
        int childstat;
        if(pid==0){
            printf("i'm child\n");
            printf("%s ex\n",dlcs[face*9+lednum]);
            char dest[100] = "./dlc/";
            strcat(dest, dlcs[face*9+lednum]);
            printf("%s\n",dest);
            execlp(dest,dest,NULL);

        }
        else{
            // int buf[6]={getpid(),0,0,0,0,0};
            // buf[3]=1;
            // memcpy(ptr,buf,sizeof(int)*5);
            wait(&childstat);
            // buf[3]=0;
            // memcpy(ptr,buf,sizeof(int)*5);
            menuinit();
            printf("%d  child end\n",childstat);
            printf("%d \n",WEXITSTATUS(childstat));
            printf("this is the parae\n");
            // exit(123);
        }
    }
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
void menuinit(){
    CubeInit();
    FILE *option;
    option=fopen("dlc.config","r");
    char buff[32];
    Color tmp={1,2,3};
    int r;
    int g;
    int b;
    int idx=0;
    while(!feof(option )){
        // fscanf(option,"%s %d %d %d",buff,&r,&g,&b);
        fscanf( option, "%s %d %d %d\n", buff, &r,&g,&b );
        printf("%s %d %d %d \n",buff,r,g,b);
        printf("newline\n");
        ++idx;
        tmp.r=r;
        tmp.g=g;
        tmp.b=b;
        SetColor(idx/9,idx%9,tmp);
        strcpy( dlcs[idx],buff); 
    }
    for(int w=0;w<10;++w){
        printf("%d %s\n",w,dlcs[w]);
    }
    // StartFadeColorCoroutine(FadeColor, face, lednum, 5000);
    
    ChangeColorImm();
    fclose(option);
    printf("start end\n");
}
void Start()
{    //위쪽 알려주기
    tone(1);
    usleep(300000);
    tone(3);
    usleep(300000);
    tone(5);
    usleep(300000);
    tone(8);
    usleep(300000);
    tone(0);
    //부팅 에니메이션
    menuinit();
    printf("tongshin start");
    // StartCoroutine(tongshin);
    printf("tongshin end\n");
    getinput();
    // SetColor(0,0, 0);
    // StartCoroutine(pressButtonsAutomatically);
}

// 매 Timing Sequence (deltaTime ms) 마다 실행된다.
void Update()
{
    int a;
}
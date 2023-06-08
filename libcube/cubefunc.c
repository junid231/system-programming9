#include "coroutine.h"
#include "cube.h"

static int diroffset[4] = { -3, +1, +3, -1 };

static int dirfaces[6][4] =
{
    {  4, 1, 5, 3 },
    { 10, 2,23, 0 },
    { 16, 3,17, 1 },
    { 22, 0,11, 2 },
    { 14,19, 0, 9 },
    {  0, 7,14,21 },
};

static int rotmat[4][9] = {
    { 0, 1, 2, 3, 4, 5, 6, 7, 8 },
    { 6, 3, 0, 7, 4, 1, 8, 5, 2 },
    { 8, 7, 6, 5, 4, 3, 2, 1, 0 },
    { 2, 5, 8, 1, 4, 7, 0, 3, 6 },
};

/* ========== Global Variables (Just used in this file) ========== */

static Color new[6][9];
static Color curr[6][9];

/* =============================================================== */

Color GetColor(int face, int lednum) {
    return new[face][lednum];
}

void SetColor(int face, int lednum, Color newcolor) {
    new[face][lednum] = newcolor;
}

// dir: up 0, right 1, down 2, left 3
Color GetDirLED(int face, int lednum, int *newface, int *newlednum, int dir) {
    *newlednum = lednum + diroffset[dir];

    // out of face
    if (*newlednum < 0 || *newlednum >= 9 || (dir % 2 == 1 && (*newlednum / 3 != lednum / 3)))
    {
        *newface = dirfaces[face][dir] % 6;
        *newlednum = (*newlednum - 3 * diroffset[dir] + 9) % 9;

        // clockwise 90 deg rotation repitition
        int rotamount = dirfaces[face][dir] / 6;
        *newlednum = rotmat[rotamount][*newlednum];
    }
    // not out of face
    else *newface = face;

    return new[*newface][*newlednum];
}

Color GetDirLEDwithDist(int face, int lednum, int *newface, int *newlednum, int dir, int dist) {
    int distleft = dist;
    Color temp = new[face][lednum];

    while(distleft != 0)
    {
        temp = GetDirLED(face, lednum, newface, newlednum, dir);
        distleft = distleft > 0 ? distleft - 1 : distleft + 1;
    }

    return temp;
}

Color GetOppositeLED(int face, int lednum, int *newface, int *newlednum) {
    Color ret = new[face][lednum];
    if      (lednum  < 3) ret = GetDirLEDwithDist(face, lednum, newface, newlednum, 0, 4);
    else if (lednum == 3) ret = GetDirLEDwithDist(face, lednum, newface, newlednum, 3, 4);
    else if (lednum == 4) ret = GetDirLEDwithDist(face, lednum, newface, newlednum, 0, 6);
    else if (lednum == 5) ret = GetDirLEDwithDist(face, lednum, newface, newlednum, 1, 4);
    else if (lednum  < 9) ret = GetDirLEDwithDist(face, lednum, newface, newlednum, 2, 4);
    return ret;
}

Color MakeColor(int r, int g, int b) {
    Color color = {r, g, b};
    return color;
}

Color MakeRandomColor() {
    Color color;
    color.r = 50 + rand() % 206;
    color.g = 50 + rand() % 206;
    color.b = 50 + rand() % 206;
    return color;
}

void FadeColor(Coroutine* coroutine) {
    BEGIN_COROUTINE(coroutine);

    coroutine->processms = 0;
    coroutine->startcolor = curr[coroutine->face][coroutine->lednum];

    // run process
    while (coroutine->processms <= coroutine->durationms)
    {
        curr[coroutine->face][coroutine->lednum].r = (int)(coroutine->startcolor.r + ((float)coroutine->processms/coroutine->durationms) * (new[coroutine->face][coroutine->lednum].r - coroutine->startcolor.r));
        curr[coroutine->face][coroutine->lednum].g = (int)(coroutine->startcolor.g + ((float)coroutine->processms/coroutine->durationms) * (new[coroutine->face][coroutine->lednum].g - coroutine->startcolor.g));
        curr[coroutine->face][coroutine->lednum].b = (int)(coroutine->startcolor.b + ((float)coroutine->processms/coroutine->durationms) * (new[coroutine->face][coroutine->lednum].b - coroutine->startcolor.b));
        coroutine->processms += deltaTime;
        YIELD(coroutine);
    }

    END_COROUTINE(coroutine);
}

void ChangeColorImm() {
    for(int face=0; face<6; face++) {
        for(int lednum=0; lednum<9; lednum++) {
            curr[face][lednum].r = new[face][lednum].r;
            curr[face][lednum].g = new[face][lednum].g;
            curr[face][lednum].b = new[face][lednum].b;
        }
    }
}

void CubeInit() {
    // Cube Color Initiate
    memset(new, 0, sizeof(Color) * 6*9);
    memset(curr, 0, sizeof(Color) * 6*9);
}

Color GetCurrColor(int face, int lednum) {
    return curr[face][lednum];
}

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

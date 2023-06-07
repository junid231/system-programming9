//C program for Producer process illustrating POSIX shared-memory API.
 
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

static int times=0;
int main()
{
    /* the size (in bytes) of shared memory object */
    const int SIZE = sizeof(int)*10;
 
    /* name of the shared memory object */
    const char* name = "input";
 
    /* shared memory file descriptor */
    int shm_fd;
 
    /* pointer to shared memory object */
    void* ptr;
 
    /* create the shared memory object */
    shm_fd = shm_open(name,  O_RDWR, 0666);
    if (shm_fd<0){
        printf("exec the menu first\n");
        exit(1);
    }
 
    /* memory map the shared memory object */
    ptr = mmap(0, SIZE,  PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd, 0);
 
    /* write to the shared memory object */
    // sprintf(ptr, "%s", message_0);
    // sprintf(ptr, "%d", getpid());
    int buf[6]={0,0,0,0,0,0};
    while(1){
        int a,b;
        scanf("%d %d",&a,&b);
        prinf("input %d %d",a,b)
        
    }
    // memcpy(ptr,buf,sizeof(int)*4);
    // printf("producer pid %d\n",getgid());
    // while(1){
    //     printf("prodsleep\n");
    //     sleep(10);
    // }
    return 0;
}
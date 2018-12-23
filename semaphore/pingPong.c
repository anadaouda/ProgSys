#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>


int main() {
    sem_t * semaphore = sem_open("/maSemaphore", O_RDWR | O_CREAT, S_IRWXU ,1);
    pid_t pid;
    int pos;
    int i;
    int ok1, ok2;
    for (i=0; i<2; i++) {
        pid=fork();
        pos = i;
        if (pid == 0) {
                sem_t * semaphore = sem_open("/maSemaphore", O_RDWR);
                assert(semaphore);

                ok1 = sem_wait(semaphore);
                printf("%i : %i\n", pos, ok1);
                fflush(stdout);
                ok2= sem_post(semaphore);
                exit(0);
        }
    }
    for (i=0; i<2; i++) {
        wait(NULL);
    }

    return 0;
}

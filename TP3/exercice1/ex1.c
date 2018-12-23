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

struct mySem_t {
    pthread_mutex_t verrou;
    pthread_cond_t cond;
    int n;
};
typedef struct mySem_t mySem_t;

void init(mySem_t * mySem, int value) {
    mySem->n = value;

    pthread_mutex_init(&(mySem->verrou), NULL);
    pthread_cond_init(&(mySem->cond), NULL);
}

void P(mySem_t * mySem) {
    pthread_mutex_lock(&(mySem->verrou));
    while(mySem->n == 0) {
        pthread_cond_wait(&(mySem->cond), &(mySem->verrou));
    }
    mySem->n--;
    pthread_mutex_unlock(&(mySem->verrou));
}

void V(mySem_t * mySem) {
    pthread_mutex_lock(&(mySem->verrou));
    mySem->n++;
    pthread_cond_signal(&(mySem->cond));
    pthread_mutex_unlock(&(mySem->verrou));

}

int main() {
    mySem_t * mySem = malloc(sizeof(mySem_t));
    init(mySem,1);

    pid_t pid;
    int pos;
    int i;
    int ok1, ok2;
    for (i=0; i<2; i++) {
        pid=fork();
        pos = i;
        if (pid == 0) {
                P(mySem);
                printf("pos = %i\n", pos);
                fflush(stdout);
                V(mySem);
                exit(0);
        }
    }
    for (i=0; i<2; i++) {
        wait(NULL);
    }

    return 0;
}

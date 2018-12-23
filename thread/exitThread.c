#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define N 4

void * fonction() {
    printf("Bonjour je suis un thread\n");
    sleep(10);
    return NULL;
}


int main(int argc, char const *argv[]) {
    pthread_t threads[N];
    void * retval;
    int i;

    for(i = 0; i < N; i++) {
        pthread_create(&threads[i], NULL, fonction, NULL);
    }
    pthread_exit(retval);
    
    return 0;
}

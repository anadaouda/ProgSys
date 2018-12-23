#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

void * fonction1(void * arg) {
    printf("Voici l'argument %i\n", *(int *)arg);
    return NULL;
}

void * fonction2(void * arg) {
    printf("Dans cette deuxieme focntion voici l'argument %i\n",*(int *)arg);
    return NULL;
}

int main(int argc, char const *argv[]) {
    pthread_t thread1;
    pthread_t thread2;

    int arg1 = 1;
    int arg2 = 2;

    pthread_create(&thread1, NULL, fonction1, (void *)&arg1);
    pthread_create(&thread2, NULL, fonction2, (void *)&arg2);

    sleep(3);

    return 0;
}

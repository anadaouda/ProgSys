#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

void * dispIndex(void * arg) {
    printf("Voici l'indice du thread : %i\n", *(int *)arg);
    return NULL;
}


int main(int argc, char const *argv[]) {
    pthread_t threads[10];
    int i, val[10];

    // i est partagé par tout le monde donc on passe par le tableau val

    for(i = 0; i<10; i++) {
        val[i] = i;
        pthread_create(&threads[i], NULL, dispIndex, (void *)&val[i]);
    }

    sleep(2);

    return 0;
}

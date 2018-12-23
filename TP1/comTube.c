#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>

#define N 3
#define M 10

void randomizeTab(int tab[]) {
    int i;
    for (i = 0; i < M; i++) {
        tab[i] = i;
    }
}

int max(int tab[], int sizeTab) {
    int max;
    int i;

    max = tab[0];

    for (i = 1; i < sizeTab; i++) {
        if (tab[i] > max) {
            max = tab[i];
        }
    }
    printf("max = %i\n", max);
    fflush(stdout);
    return max;
}

void closeUselessFd(int slaveNo, int toSlave[][2], int toMaster[][2]) {
    int j;
    for (j = 0; j < N-1; j++) {
        if (j != slaveNo) {
            close(toMaster[slaveNo][0]);
            close(toSlave[slaveNo][0]);
            close(toMaster[slaveNo][1]);
            close(toSlave[slaveNo][1]);
        } else {
            close(toSlave[slaveNo][1]);
            close(toMaster[slaveNo][0]);
        }
    }
}

int main(int argc, char const *argv[]) {

    pid_t pid;
    int i, j, k, localMax, nbRead, written, subTabSize, res;
    int tab[M], localMaxRes[N-1];
    int * subTab;

    randomizeTab(tab);

    int toSlave[N-1][2]; // tubes du maitre vers les esclaves
    int toMaster[N-1][2]; // tubes des esclaves vers le maitre


// creation de tous les tubes
for (i = 0; i < N-1; i++) {
    pipe(toSlave[i]);
    pipe(toMaster[i]);
}

// supression des tubes inutiles
for (j = 0; j < N-1; j++) {
    pid = fork();

    if (pid == 0) {
        closeUselessFd(j, toSlave, toMaster);
        break;
    } else {
        close(toSlave[j][0]);
        close(toMaster[j][1]);
    }
}


    for(k = 0; k < N-1; k++) {
        if (pid == 0) {
            nbRead = read(toSlave[k][0], &subTabSize, sizeof(int));
            nbRead = read(toSlave[k][0], subTab, M/N*sizeof(int));
            localMax = max(subTab, subTabSize);

            printf("Le maximum local du sous-tableau %i est %i", k, localMax);
            fflush(stdout);

            written = write(toMaster[k][1], &localMax, sizeof(int));

            exit(0);
        }
        else {
            // détermination de la taille du sous-tableau
            res = (k+1)*(M/(N-1) + 1);
            if (res > M) {
                subTabSize = res-10;
            } else {
                subTabSize = M/N + 1;
            }

            subTab = &tab[k*subTabSize];

            written = write(toSlave[k][1], &subTabSize, subTabSize*sizeof(int));
            written = write(toSlave[k][1], subTab, subTabSize*sizeof(int));

            nbRead = read(toMaster[k][0], &localMax, sizeof(int));

            localMaxRes[k] = localMax;
        }
    }

printf("Le maximum est donc : %i\n", max(localMaxRes, N-1));


    return 0;

}

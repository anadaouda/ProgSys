#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

void morse(int signum) {
    if (signum == SIGALRM) {
        printf(" ");
        fflush(stdout);
    }
    if (signum == SIGUSR1) {
        printf(".");
        fflush(stdout);
    }
    if (signum == SIGUSR2) {
        printf("_");
        fflush(stdout);
    }
    return;
}

int main(int argc, char const *argv[]) {
    int test;
    struct sigaction structSig;

    structSig.sa_handler = &morse;

    test = sigaction(SIGALRM, &structSig, NULL);
    test = sigaction(SIGUSR1, &structSig, NULL);
    test = sigaction(SIGUSR2, &structSig, NULL);

    printf("PID = %i\n",getpid());
    fflush(stdout);

    while(1);

    return 0;
}

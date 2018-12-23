#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>

void handler (int signum) {
    char str[] = "coucou";
    write(STDOUT_FILENO, (void*)str, sizeof(str));
}

int main(int argc, char const *argv[]) {
    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    printf("Mon PID : %i\n", getpid());
    action.sa_handler = handler;
    sigaction(SIGUSR1, &action, NULL);
    pause();
    exit(0);
}

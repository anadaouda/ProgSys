#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

int morse(char signum) {
    if (signum == ' ') {
        return SIGALRM;
    }
    else if (signum == '.') {
        return SIGUSR1;
    }
    else if (signum == '_') {
        return SIGUSR2;
    }
    else {
        return SIGTERM;
    }
}

int main(int argc, char const *argv[]) {
    char * message = malloc(100);
    int pid, i, len, send;

    printf("PID = %i\n",getpid());
    fflush(stdout);

    printf("Message a envoyer\n");
    fgets(message,100,stdin);

    printf("PID\n");
    scanf("%i", &pid);

    len = strlen(message);

    for (i = 0; i<len; i++) {
        sleep(1);
        if (kill(pid, morse(message[i]))) {
            perror("Send");
            exit(0);
        }
    }
    free(message);

    return 0;
}

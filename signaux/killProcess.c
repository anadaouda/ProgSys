#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char const *argv[]) {
    pid_t pid = getpid();
    pid_t pid2 = fork();
    int i;
    if (pid2 ==0) {
        while(kill(pid, SIGKILL));
    } else {
        int i = 0;
        while(++i);
    }
    return 0;
}

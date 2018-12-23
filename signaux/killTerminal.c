#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char const *argv[]) {
    pid_t pid = getpid();
    printf("Mon pid : %i\n", pid);
    while(1);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char const *argv[]) {
    void * ptr = NULL;
    pid_t pid;
    ptr = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    pid = fork();
    if (pid > 0) {
        char * base_ptr = (char *)ptr;
        char * str = "coucou";
        strcpy(base_ptr, str);
        wait(NULL);
    } else if (pid == 0) {
        char * base_ptr = (char *)ptr;
        fprintf(stdout, "%s\n", base_ptr);
    }
    return 0;
}

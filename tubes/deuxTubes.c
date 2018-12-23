#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <limits.h>
#include <poll.h>
#include <fcntl.h>

int main(int argc, char * argv[]) {
    int fd[2];
    pid_t pid;
    pipe(fd);
    pid = fork();
    if (pid>0) {
        close(fd[1]);
        close(STDIN_FILENO);
        dup(fd[0]);
        close(fd[0]);
        // on fait ça pour remplacer l'entree standard
        execlp("wc", "wc", "-l", NULL);
    } else if (pid == 0) {
        int fd2[2];
        pid_t pid2;
        close(fd[0]);
        pipe(fd2);
        pid2 = fork();
        if(pid2 > 0) {
            close(fd2[1]);
            close(STDOUT_FILENO);
            dup(fd[1]);
            close(fd[1]);
            close(STDIN_FILENO);
            dup(fd2[0]);
            close(fd2[0]);
            execlp("grep", "grep", "machin", NULL);
        }
        else if (pid == 0) {
            close(fd2[0]);
            close(fd[1]);
            close(STDOUT_FILENO);
            dup(fd2[1]);
            close(fd2[1]);
            execlp("cat", "cat", "toto.txt", NULL);
        }
    }
    return 0;
}

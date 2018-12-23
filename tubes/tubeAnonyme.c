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


#define PIPE_SIZE_MAX 100

int main() {
  int pipeFd[2];
  if (pipe(pipeFd) == -1) {
    exit(1);
  }

  // pipefd[0] = extremité en lecture
  // pipefd[1] = extremité en ecriture

  pid_t pid = fork();

  if (pid == 0) {
    // le fils ecrit dans la pipe
    int written;
    close(pipeFd[0]);
    written = write(pipeFd[1], "slt", 3);
  }
  else {
    // lire dans la pipe
    int nbRead;
    char * buffer = malloc(PIPE_SIZE_MAX);
    close(pipeFd[1]);
    nbRead = read(pipefd[0], buffer, PIPE_SIZE_MAX);
    printf("J'ai recu : %s\n", buffer);
    free(buffer);
  }

  return 1;

}

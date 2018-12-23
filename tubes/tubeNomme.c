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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PIPE_SIZE_MAX 100

int main() {

  pid_t pid = fork();

  if (pid == 0) {
    // le fils ecrit dans la pipe
    int written;
    int pipeFd;
    pipeFd = open("unTubeNomme", O_WRONLY);
    written = write(pipeFd, "slt", 3);
  }
  else {
    // lire dans la pipe
    int nbRead;
    int pipeFd;
    char * buffer = malloc(PIPE_SIZE_MAX);
    mkfifo("unTubeNomme", S_IRWXU);
    pipeFd = open("unTubeNomme", O_RDONLY);
    nbRead = read(pipeFd, buffer, PIPE_SIZE_MAX);
    printf("J'ai recu : %s\n", buffer);
    free(buffer);
  }

  return 1;

}

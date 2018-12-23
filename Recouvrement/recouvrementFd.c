#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char ** argv) {

  pid_t pid = fork();

  if (pid == 0) {
    execl("./pgDispFd","./pgDispFd", NULL);
    exit(1);
  }

  return 0;
}

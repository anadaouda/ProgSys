#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>


int main() {
  pid_t pid;
  int i;
  for (i = 0; i < 10; i++) {
    pid = fork();

    if (pid == 0) {
      printf("Processus : %i, mon pid est %i et mon pere est %i\n", i, getpid(), getppid());
      //fflush(stdout);
      break;
    }
  }
  return 0;
}

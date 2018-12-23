#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
  pid_t pid = fork();
  if (pid == 0 ) {
    exit(13);
  }
  else if (pid > 0){
    pid_t fils;
    int status;
    int toto = 0;
    while(++toto);
    fils = wait(&status);
    printf("Mon fils %i est fini avec %i comme valeur de retour.\n", fils, WEXITSTATUS(status));
    fflush(stdout);
  }
  return 0;
}

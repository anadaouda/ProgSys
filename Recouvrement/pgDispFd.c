#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
  int fd = open("randFile.txt", O_RDONLY | O_CLOEXEC);
  printf("fd = %i\n", fd);
  fflush(stdout);

  return 0;
}

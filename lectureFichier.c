#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc,char** argv) {
  FILE * file = fopen("lectureFichier.txt", "r");


  int size = lseek(fileno(file), 0, SEEK_END);
  rewind(file);

  char ** text = malloc(size * sizeof(char *));

  int wordCount = 0;
  int wordLetterCount = 0;
  int nbLines = 0;
  char letter;

  text[wordCount] = malloc(size*sizeof(char));

  do {
    letter = fgetc(file);

    if (letter == '\n') {
      text[wordCount][wordLetterCount] = '\0';
printf("%s\n", text[wordCount]);
      nbLines++;
      wordCount++;
      wordLetterCount = 0;
      text[wordCount] = malloc(size*sizeof(char));
    }
    else if (letter == ' ') {
      text[wordCount][wordLetterCount] = '\0';
printf("%s ", text[wordCount]);
      wordCount++;
      wordLetterCount = 0;
      text[wordCount] = malloc(100*sizeof(char));
    }
    else {
      text[wordCount][wordLetterCount] = letter;
      wordLetterCount++;
    }

  } while(!feof(file));

  //printf("nbLines = %i\n", nbLines);

  fclose(file);
  //free(text);

  return 0;
}

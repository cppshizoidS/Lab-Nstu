#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#define SIZE 256

int main(int argc, char *argv[]) {
  int inputFd, outputFd, openFlags;
  mode_t filePerms;
  char buf, str[SIZE], newStr[SIZE];

  if (argc != 3) {
    printf("To use this programm: <prog_name> <file_name> <spaces_to_delete>\n");
    exit(-1);
  }

  inputFd = open(argv[1], O_RDONLY);

  if (inputFd < 0) {
    printf("Error %d (%s) while open file: %s!\n",errno, strerror(errno), argv[1]);
    exit(-2);
  }

  int charNums = 0;

  while (read(inputFd, &buf, 1) > 0) {
    str[charNums] = buf;
    charNums++;
  }

  close(inputFd);

  int spaces = 0;
  int toDelete = (int) *argv[2] - 48;

  if (toDelete < 0) {
    printf("Expected positive number of spaces\n");
    exit(-1);
  }

  for (int i = 0; i < charNums; i++) {
    if (str[i] == ' ' && spaces < toDelete) spaces++;
    else newStr[i - spaces] = str[i];
  }
  newStr[charNums - spaces] = '\0';
    
    
  openFlags = O_CREAT | O_WRONLY | O_TRUNC;
  filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;


  outputFd = open("./build/lab1.text", openFlags, filePerms);

  if (outputFd == -1) {
    printf ("Error opening file %s\n ", "./build/lab1.text"); 
    exit(-3);
  }

  if(!write(outputFd, newStr, strlen(newStr))) {
    printf("Wild Error Appear!");
  }

  close (outputFd);
  printf("Program Done Sucessfully");

  return 0;
}

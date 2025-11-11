#include <asm-generic/errno-base.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

char checkdir[] = ".check";

int createRepo() {
  
  int repo;
  repo = mkdir(checkdir, 0777);

  if (repo == 0) {
    printf("checkpoint repo has been created\n");
    return 0;
  } else if (repo == -1 & errno == 17) {
    printf("repo exists already. moving on!\n");
    return 0;
  } else {
    printf("something went wrong... i'mma head out.\n");
    return 1;
  }

} 

int main() {
  int repo;

  repo = createRepo();

  if (repo != 0) {
    return 1;
  }

  return 0;
}

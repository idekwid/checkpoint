#include <asm-generic/errno-base.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

int create_repo()
{
  char checkdir[] = ".check";
  
  int repo = mkdir(checkdir, 0777);

  if (repo == 0)
  {
    printf("checkpoint repo has been created\n");
    return 0;
  }
  else if ((repo == -1) & (errno == 17))
  {
    printf("repo exists already. moving on!\n");
    return 0;
  }
  else
  {
    printf("something went wrong... i'mma head out.\n");
    return 1;
  }
}

int create_meta_file()
{
  FILE *meta_file = fopen(".check/meta.check", "r");

  if (meta_file == NULL)
  {    
    meta_file = fopen(".check/meta.check", "w");
    fprintf(meta_file,"meta file!");
    
    printf("meta file created\n");
    
    fclose(meta_file);
    return 0;
  }
  else
  {
    printf("meta file exists, so moving on\n");
    fclose(meta_file);
    return 0;
  }

  printf("something somewhere went really wrong...");
  fclose(meta_file); // this might throw a segfault but I can't prove it
  return 1;
}

int get_file_and_put()
{
  FILE *main = fopen(".check", "r");
  
  if (main == NULL)
  {
    printf("repo does not exist, run 'init' first.\n");
    return 1;
  }
  else
  {
    fclose(main);
    main = fopen("main.c", "r");

    fseek(main, 0, SEEK_END);
    int64_t fileSize = ftell(main);
    rewind(main);
    char* fileContent = (char*)malloc(sizeof(char)*fileSize);
    fread(fileContent, 1, fileSize, main);

    fclose(main);
    main = fopen(".check/main.c.check", "w");

    fprintf(main, "%s", fileContent);

    free(fileContent);
    fclose(main);
    return 0;
  }
}

void help_text()
{
  printf("Checkpoint\n\n");
  printf("Options:\n");
  printf("init - initializes the repository. run this before anything\n");
  printf("point - save current main file to the repo\n");
  printf("help - show this screen\n");
}

int main(int argc, char* argv[]) {
  if (argv[1] == NULL)
  {  
    printf("unknown command - run help to see what you can do\n");
    return 1;
  }
  else
  {
    if (strcmp(argv[1], "init") == 0)
    {
      int repo = create_repo();
      if (repo != 0) return 1;

      int meta_file = create_meta_file();
      if (meta_file != 0) return 1;

      return 0;
    } 
    else if (strcmp(argv[1], "point") == 0)
    {
      int gotFile = get_file_and_put();
      if (gotFile != 0) return 1;
      
      return 0;
    } 
    else if (strcmp(argv[1], "help") == 0)
    {
      help_text();
      return 0;
    }
    else
    {
      printf("unknown command - run help to see what you can do\n");
      return 1;
    }
  }

  return 0;
}

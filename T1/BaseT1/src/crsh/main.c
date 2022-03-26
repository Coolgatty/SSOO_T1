#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> 
#include <string.h>
#include <stdlib.h>
#include<sys/wait.h>

#include "../input_manager/manager.h"

void init_process(char** input)
{
  int status;
  pid_t p = fork();
  if( p == 0)
  {
    if(strcmp(input[0], "crexec") == 0)
    {
      if(strcmp(input[1], "hello") == 0)
      {
        printf("Hello World!\n");
      }
      else if(strcmp(input[1], "sum") == 0)
      {

      }
      else if(strcmp(input[1], "is_prime") == 0)
      {

      }
      else
      {
        printf("\x1b[31m No process with name '%s'\n\033[0m", input[1]);
        exit(404);
      }
      exit(0);
    }
    else if(strcmp(input[0], "crexit") == 0)
    {
      exit(9);
    }
    exit(0);
  }
}

int main(int argc, char const *argv[])
{
  int status;
  while(1)
  {
    printf("> ");
    char **input = read_user_input();
    printf("> The first argument you wrote was: %s\n", input[0]);
    init_process(input);
    wait(&status);
    printf("Child exit code is: %d\n", WEXITSTATUS(status));
    if (WEXITSTATUS(status) == 9)
    {
      break;
    }
    free_user_input(input);
  }
}


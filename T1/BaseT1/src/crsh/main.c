
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>

#include "../input_manager/manager.h"

void hello()
{
  printf("\033[32mHello World!\n\033[0m");
}

float sum(float a, float b)
{
  return a + b;
}

int is_prime(char* input)
{
  char* ptr;
  unsigned long number = strtoul(input, &ptr, 10);
  if (number == 2)
  {
    return 1;
  }
  if (number == 1)
  {
    return 0;
  }
  if(number % 2 == 0)
  {
    return 0;
  }
  else
  {
    for (int i = 3; i*i <= number; i += 2)
    {
      if(number % i == 0)
      {
        return 0;
      }
    }
  }
  return 1;
}

void init_process(char** input)
{
  time_t begin = time(NULL);
  pid_t p = fork();
  if( p == 0)
  {
    if(strcmp(input[0], "crexec") == 0)
    {
      int status;
      char** args = input + 1;
      char* programName = args[0];
      exit(execvp(programName, args));
    }
    else if(strcmp(input[0], "hello") == 0)
    {
      hello();
    }
    else if(strcmp(input[0], "sum") == 0)
    {
      float result = sum(atof(input[1]), atof(input[2]));
      printf("\033[32m %s + %s = %f \n\033[0m", input[1], input[2], result);
    }
    else if(strcmp(input[0], "is_prime") == 0)
    {
      int prime = is_prime(input[1]);
      if(prime)
      {
        printf("\033[32m Number %s is prime \n\033[0m", input[1]);
      }
      else
      {
        printf("\033[33m Number %s is not prime \n\033[0m", input[1]);
      }
    }
    else if (strcmp(input[0], "crlist") == 0)
    {
      // TODO: 
    }
    else if (strcmp(input[0], "crexit") == 0)
    {
       // TODO:
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
    pid_t pid = wait(&status);
    printf("Process %d exited with code: %d\n", pid, WEXITSTATUS(status));
    if (WEXITSTATUS(status) == 9)
    {
      break;
    }
    if(WEXITSTATUS(status) == 255)
    {
      printf("\x1b[31m No process with name '%s'\n\033[0m", input[1]);
    }
    free_user_input(input);
  }
}



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

#include "../process/process.h"
#include "../linked_list/linked_list.h"

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
  if(number % 2 == 0)
  {
    return 0;
  }
  else
  {
    for (int i = 3; i*i < number; i += 2)
    {
      if(number % i == 0)
      {
        return 0;
      }
    }
  }
  return 1;
}

void init_process(char** input, PList* process_list)
{
  pid_t process_pid = fork();
  time_t start = time(NULL);
  if( process_pid == 0)
  {
    if(strcmp(input[0], "crexec") == 0)
    {
      char** args = input + 1;
      char* programName = args[0];
      char* prefix = "./";
      char target[strlen(programName)+strlen(prefix)];
      if (strstr(programName, "./") == NULL) 
      {
        strcpy(target, prefix);
        strcat(target, programName);
        args[0] = target;
        exit(execvp(target, args));
      }
      else
      {
        exit(execvp(programName, args));
      }
      
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
      Node* node = process_list->first;
      while(node)
      {
          Node* next = node->next;
          Process* process = node->process;
          time_t end = time(NULL);
          int exec_time = (end - process->timestart);
          printf("\033[33m Process %s with pid %d has been running for %i seconds.\n", process->name, process->pid, exec_time);
          node = next;
      }  
    }
    else if (strcmp(input[0], "crexit") == 0)
    {
      printf("here\n");
      exit(9);
    }
    exit(0);
  }
  else if(process_pid > 0)
  {
    char** args;
    if(strcmp(input[0], "crexec") == 0 && input[1])
    {
      args = input + 1;
    }
    else if (strcmp(input[0], "sum") == 0 || strcmp(input[0], "hello") == 0 || strcmp(input[0], "is_prime") == 0)
    {
      args = input;
    }
    else
    {
      return;
    }
    char* programName = args[0];
    char* prefix = "./";
    char target[strlen(programName)+strlen(prefix)];
    if (strstr(programName, "./") == NULL) 
    {
      strcpy(target, prefix);
      strcat(target, programName);
    }
    Process* process = process_init(process_pid, start, target);
    printf("Started process with name: %s and id: %d\n", process->name, process->pid);
    list_append(process_list, process);
    if(input)
    {
      free_user_input(input);
      input = NULL;
    }
  }
}

volatile sig_atomic_t stop = 0;

// Handler for child death signal
void pingall(int i)
{
  int status;
  pid_t pid = waitpid(-1, &status, WNOHANG); // wait until child terminates or changes its status
  if(pid > 0)
  {
    printf("Process %d exited with code: %d\n", pid, WEXITSTATUS(status));
  }
  if(WEXITSTATUS(status) == 9)
  {
    stop = 1;
  }
}

void invalid(int i)
{
  int status;
  waitpid(-1, &status, WNOHANG); // wait until child terminates or changes its status
  if(WEXITSTATUS(status) == 255)
  {
    printf("\033[31m Invalid or non-existant process name.\033[0m\n");
  }
}

int main(int argc, char const *argv[])
{
  int status;

  PList* process_list = list_init();

  char** input = NULL;

  // Ping process to see if its still running (Taken from https://stackoverflow.com/questions/27342046/how-to-handle-sighld)

  signal(SIGCHLD, pingall); // catch child status changes
  do {
      waitpid(-1, &status, WNOHANG); // wait until child terminates or changes its status
  } while(!WIFEXITED(status));

  signal(SIGCHLD, invalid); // catch child status changes
  do {
      waitpid(-1, &status, WNOHANG); // wait until child terminates or changes its status
  } while(!WIFEXITED(status));

  while(!stop)
  {
    printf("> ");
    input = read_user_input();
    printf("> The first argument you wrote was: %s\n", input[0]);

    init_process(input, process_list);
  }
  if(input)
  {
    free_user_input(input);
  }
  list_destroy(process_list);
}


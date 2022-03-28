#include "process.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Process* process_init(pid_t pid, time_t time, char* name)
{
    Process* process = malloc(sizeof(Process));
    process->pid = pid;
    process->timestart = time;
    char* pname = malloc(sizeof(name));
    strcpy(pname, name);
    process->name = pname;
    process->ended = 0;
    return process;
}

void process_destroy(Process* process)
{
    if(process)
    {
        free(process->name);
        free(process);
    }
    else
    {
        printf("=======================================================\n");
        printf("\033[0;31mWARNING:\033[0m You are trying to free a NULL pointer!\nLine %d in %s\n", __LINE__, __FILE__);
        printf("=======================================================\n");
    }
}
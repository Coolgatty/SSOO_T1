#pragma once

#ifndef PROCESS_H
#define PROCESS_H

#include <time.h>

typedef struct process
{
    pid_t pid;
    time_t timestart;
    int ended;
    char* name;
} Process;

Process* process_init(pid_t pid, time_t time, char* name);

void process_destroy(Process* process);

#endif
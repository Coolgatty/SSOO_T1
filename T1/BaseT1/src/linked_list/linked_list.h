#pragma once

#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include "../process/process.h"

typedef struct node
{
    Process* process;
    struct node* next;
    struct node* previous;
} Node;

typedef struct
{
    Node* first;
    Node* last;
} PList;


Node* node_init(Process* process);
void node_destroy(Node* node);

PList* list_init();
void list_destroy(PList* list);
void list_append(PList* list, Process* process);
int list_is_empty(PList* list);
Process* list_pop(PList* list, int index);
Process* list_peek(PList* list, int index);
int list_contains(PList* list, pid_t pid);

#endif
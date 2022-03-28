#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

Node* node_init(Process* process)
{
    Node* node = malloc(sizeof(Node));
    node->process = process;
    node->next = NULL;
    node->previous = NULL;
    return node;
}

void node_destroy(Node* node)
{
    if(node)
    {
        process_destroy(node->process);
        free(node);
    }
    else
    {
        printf("=======================================================\n");
        printf("\033[0;31mWARNING:\033[0m You are trying to free a NULL pointer!\nLine %d in %s\n", __LINE__, __FILE__);
        printf("=======================================================\n");
    }
}

PList* list_init()
{
    PList* list = malloc(sizeof(PList));
    list->first = NULL;
    list->last = NULL;
    return list;
}

void list_append(PList* list, Process* process)
{
    Node* node = node_init(process);
    if(!list->first)
    {
        list->first = node;
        list->last = node;
    }
    else if(list->first == list->last)
    {
        list->first->next = node;
        list->last = node;
        node->previous = list->first;
    }
    else
    {
        Node* previous = list->last;
        previous->next = node;
        list->last = node;
        node->previous = previous;
    }
}

int list_is_empty(PList* list)
{
    if(!list->first && !list->last)
    {
        return 1;
    }
    return 0;
}


Process* list_pop(PList* list, int index)
{
    if(list_is_empty(list))
    {
        // printf("=======================================================\n");
        // printf("\033[0;31mERROR:\033[0m Index %i out of range!\nLine %d in %s\n", index, __LINE__, __FILE__);
        // printf("=======================================================\n");
        return NULL;
    }
    if(index == 0)
    {
        Node* node = list->first;
        list->first = node->next;
        if(list->first)
        {
            list->first->previous = NULL;
        }
        else
        {
            list->last = NULL;
        }
        Process* process = node->process;
        node_destroy(node);
        return process;
    }
    else if(index == -1)
    {
        Node* node = list->last;
        list->last = node->previous;
        if(list->last)
        {
            list->last->next = NULL;
        }
        else
        {
            list->first = NULL;
        }
        Process* process = node->process;
        node_destroy(node);
        return process;
    }
    else
    {
        Node* node = list->first;
        for(int i = 0; i < index; i++)
        {
            if(!node)
            {
                printf("=======================================================\n");
                printf("\033[0;ERROR:\033[0m Index %i out of range!\nLine %d in %s\n", index, __LINE__, __FILE__);
                printf("=======================================================\n");
                return NULL;
            }
            node = node->next;
        }
        node->previous->next = node->next;
        node->next->previous = node->previous;
        Process* process = node->process;
        node_destroy(node);
        return process;
    }
}

Process* list_peek(PList* list, int index)
{
    if(list_is_empty(list))
    {
        printf("=======================================================\n");
        printf("\033[0;ERROR:\033[0m Index %i out of range!\nLine %d in %s\n", index, __LINE__, __FILE__);
        printf("=======================================================\n");
        return NULL;
    }
    if(index == 0)
    {
        return list->first->process;
    }
    else if(index == -1)
    {
        return list->last->process;
    }
    else
    {
        Node* node = list->first + sizeof(Node) * index;
        if(!node)
        {
            printf("=======================================================\n");
            printf("\033[0;ERROR:\033[0m Index out of range!\nLine %d in %s\n", __LINE__, __FILE__);
            printf("=======================================================\n");
            return NULL;
        }
        return node->process;
    }
}


int list_contains(PList* list, pid_t pid)
{
    Node* current = list->first;
    while(current)
    {
        if(current->process->pid == pid)
        {
            return 1;
        }
        current = current->next;
    }
    return 0;
}


void list_destroy(PList* list)
{
    if(list)
    {
        Node* node = list->first;
        while(node)
        {
            Node* next = node->next;
            node_destroy(node);
            node = next;
        }
        free(list);
    }
    else
    {
        printf("=======================================================\n");
        printf("\033[0;31mWARNING:\033[0m You are trying to free a NULL pointer!\nLine %d in %s\n", __LINE__, __FILE__);
        printf("=======================================================\n");
    }
}
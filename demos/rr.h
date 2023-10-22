//
// Created by pauls on 10/20/2023.
//
#ifndef PROJECT2_RR_H
#define PROJECT2_RR_H

#include "lwp.h"

typedef struct Node {
   thread data;
   struct Node* next;
} Node;

typedef struct {
   Node* head;
   Node* tail;
   int count;
} ThreadQueue;

extern ThreadQueue threadQueue;
extern scheduler activeScheduler;
extern thread current_thread;
extern tid_t last_tid;

void initQueue();
void enqueue(thread new_thread);
thread next();
int qlen();
void remove_thread(thread victim_thread);
void init();
void shutdown();
void admit(thread newThread);





#endif //PROJECT2_RR_H

//
// Created by pauls on 10/20/2023.
//
#ifndef PROJECT2_RR_H
#define PROJECT2_RR_H
#include <stddef.h>
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
void rr_admit(thread new_thread);
thread rr_next();
int rr_qlen();
void rr_remove(thread victim_thread);
void init_thread();
void shutdown();
void admit(thread newThread);

extern struct scheduler rr_publish;
extern scheduler activeScheduler;



#endif //PROJECT2_RR_H

#include "rr.h"
#include <stdio.h>
#include <stdlib.h>

ThreadQueue threadQueue;
thread current_thread = NULL;
tid_t last_tid = 0;
struct scheduler rr_publish = {NULL, NULL, rr_admit, rr_remove, rr_next, rr_qlen};
scheduler activeScheduler = &rr_publish;

// Initialize the thread queue
void initQueue() {
   printf("Initializing queue...\n");
   threadQueue.head = NULL;
   threadQueue.tail = NULL;
   threadQueue.count = 0;
}

// Add a new thread to the queue
void enqueue_thread(thread new_thread) {
   printf("Enqueueing thread %lu...\n", new_thread->tid);
   Node *newNode = malloc(sizeof(Node));
   newNode->data = new_thread;
   newNode->next = NULL;

   if (threadQueue.head == NULL) {
      threadQueue.head = newNode;
      threadQueue.tail = newNode;
   } else {
      threadQueue.tail->next = newNode;
      threadQueue.tail = newNode;
   }
   threadQueue.count++;
}

// Get the next thread to run
thread rr_next() {
   printf("Getting next thread to run...\n");
   if (threadQueue.count == 0) {
      return NULL;
   }
   Node *temp = threadQueue.head;
   thread nextThread = temp->data;
   threadQueue.head = threadQueue.head->next;
   return nextThread;
}

// Get the length of the queue
int rr_qlen() {
   printf("Getting queue length...\n");
   return threadQueue.count;
}

// Remove a thread from the queue
void rr_remove(thread victim_thread) {
   printf("Removing thread %lu...\n", victim_thread->tid);
   Node *temp = threadQueue.head;
   Node *prev = NULL;

   while (temp != NULL) {
      if (temp->data->tid == victim_thread->tid) {
         if (prev == NULL) {
            threadQueue.head = temp->next;
         } else {
            prev->next = temp->next;
         }
         free(temp);
         threadQueue.count--;
         return;
      }
      prev = temp;
      temp = temp->next;
   }
}

// Initialize scheduler-specific structures
void init_thread() {
   printf("Initializing scheduler...\n");
   initQueue();
}

// Clean up resources and free memory
void shutdown_rr() {
   printf("Shutting down scheduler...\n");
   // TODO: Free all allocated memory
}

// Admit a new thread to the scheduler
void rr_admit(thread newThread) {
   enqueue_thread(newThread);
}


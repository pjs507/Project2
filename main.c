#include <stdio.h>
#include <stdlib.h>
#include "lwp.h"

#define MAX_THREADS 100

typedef struct Node {
   thread data;
   struct Node* next;
} Node;

typedef struct {
   Node* head;
   Node* tail;
   int count;
} ThreadQueue;

ThreadQueue threadQueue;
scheduler activeScheduler = NULL;
thread current_thread = NULL; // Global variable to keep track of the current thread
tid_t last_tid = 0;

// Initialize the thread queue
void initQueue() {
   printf("Initializing queue...\n");
   threadQueue.head = NULL;
   threadQueue.tail = NULL;
   threadQueue.count = 0;
}

// Add a new thread to the queue
void enqueue(thread new_thread) {
   printf("Enqueueing thread %lu...\n", new_thread->tid);
   Node* newNode = malloc(sizeof(Node));
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
thread next() {
   printf("Getting next thread to run...\n");
   if (threadQueue.count == 0) {
      return NULL;
   }
   Node* temp = threadQueue.head;
   thread nextThread = temp->data;
   threadQueue.head = threadQueue.head->next;
   free(temp);
   threadQueue.count--;
   return nextThread;
}

// Get the length of the queue
int qlen() {
   printf("Getting queue length...\n");
   return threadQueue.count;
}

// Remove a thread from the queue
void remove_thread(thread victim_thread) {
   printf("Removing thread %lu...\n", victim_thread->tid);
   Node* temp = threadQueue.head;
   Node* prev = NULL;

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
void init() {
   printf("Initializing scheduler...\n");
   initQueue();
}

// Clean up resources and free memory
void shutdown() {
   printf("Shutting down scheduler...\n");
   // TODO: Free all allocated memory
}

// Admit a new thread to the scheduler
void admit(thread newThread) {
   printf("Admitting new thread...\n");
   enqueue(newThread);
}

struct scheduler rr_scheduler = {
   .init = init,
   .shutdown = shutdown,
   .admit = admit,
   .remove = remove_thread,
   .next = next,
   .qlen = qlen
};

// Set the scheduler
void lwp_set_scheduler(scheduler newScheduler) {
   printf("Setting the scheduler...\n");
   activeScheduler = newScheduler;
   printf("Scheduler set successfully.\n");
}

// Create a new thread
tid_t lwp_create(lwpfun function, void *arg) {
   // Allocate memory for the new thread context
   thread new_thread = malloc(sizeof(context));
   if (new_thread == NULL) {
      return NO_THREAD; // Return an error code if allocation fails
   }

   // Initialize the thread context
   // TODO: initialize with correct stack
   new_thread->tid = ++last_tid;  // Assign a unique thread ID
   new_thread->stacksize = 8; /* some stack size, e.g., 4096 */;
   new_thread->stack = malloc(new_thread->stacksize );  // Allocate stack memory
   if (new_thread->stack == NULL) {
      free(new_thread);
      return NO_THREAD;  // Return an error code if stack allocation fails
   }
   // TODO: Initialize the registers in new_thread->state
   // TODO: Set up the initial stack frame to make sure the thread starts executing 'function'

   // Add the new thread to the queue
   enqueue(new_thread);

   return new_thread->tid;
}

// Exit the current thread
void lwp_exit(int status) {
   // TODO: Remove the current thread from the queue
   // TODO: Free resources
   // TODO: Switch to the next thread
}

// Start the scheduler
void lwp_start() {
   printf("Starting the LWP...\n");
   // TODO: Get the first thread from the queue and start executing it
}

int main() {
   printf("Setting scheduler...\n");
   lwp_set_scheduler(&rr_scheduler);
   // TODO: Create threads using lwp_create()
   printf("Starting LWP library...\n");
   lwp_start();
   return 0;
}

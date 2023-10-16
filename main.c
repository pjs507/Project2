#include "include/lwp.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_THREADS 100

// Thread queue as an array
thread threadQueue[MAX_THREADS];
int currentThreadIndex = 0;  // Index of the current thread
int count = 0;  // Number of threads in the queue

scheduler current_scheduler = NULL;

// Initialize the queue
void initQueue() {
   printf("Initializing queue...\n");
   currentThreadIndex = 0;
   count = 0;
}

// Add a thread to the queue
void enqueue(thread new_thread) {
   printf("Enqueueing thread %lu...\n", new_thread->tid);
   threadQueue[count++] = new_thread;
}

// Get the next thread to run
thread next() {
   printf("Getting next thread to run...\n");
   if (count == 0) {
      return NULL;
   }
   thread nextThread = threadQueue[currentThreadIndex];
   currentThreadIndex = (currentThreadIndex + 1) % count;
   return nextThread;
}

// Get the number of threads in the queue
int qlen() {
   printf("Getting queue length...\n");
   return count;
}

// Initialize scheduler
void init() {
   printf("Initializing scheduler...\n");
   initQueue();
}

// Shutdown scheduler
void shutdown() {
   printf("Shutting down scheduler...\n");
   // TODO: Clean up any resources
}

// Admit a new thread
void admit(thread new_thread) {
   printf("Admitting new thread...\n");
   enqueue(new_thread);
}

// Remove a thread
void remove_thread(thread victim_thread) {  // Renamed to remove_thread
   printf("Removing thread %lu...\n", victim_thread->tid);
   // TODO: Implement logic to remove thread
}

// Scheduler struct
struct scheduler rr_scheduler = {
   .init = init,
   .shutdown = shutdown,
   .admit = admit,
   .remove = remove_thread,
   .next = next,
   .qlen = qlen
};


// Function to set the scheduler
void lwp_set_scheduler(scheduler new_scheduler) {
   printf("Setting the scheduler...\n");

   // TODO: Initialize the new scheduler
   // TODO: Transfer threads from the old scheduler to the new one
   // TODO: Shutdown the old scheduler

   current_scheduler = new_scheduler;

   printf("Scheduler set successfully.\n");
}

// Function to start the LWP (Lightweight Process)
void lwp_start() {
   printf("Starting the LWP...\n");

   // TODO: Convert the calling thread into a LWP
   // TODO: Admit it to the scheduler
   // TODO: Yield control to the next thread as indicated by the scheduler

   printf("LWP started successfully.\n");
}


// Main function for testing
int main() {
   printf("Setting scheduler...\n");
   lwp_set_scheduler(&rr_scheduler);

   // TODO: Create threads using lwp_create()

   printf("Starting LWP library...\n");
   lwp_start();

   return 0;
}

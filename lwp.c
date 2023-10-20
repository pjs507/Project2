// lwp.c
#include "lwp.h"
#include <stdlib.h>
#include <stdio.h>
#include "scheduler.h"

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


tid_t lwp_gettid(void) {
   // ... your implementation ...
}

void lwp_yield(void) {
   // ... your implementation ...
}


// Start the scheduler
void lwp_start() {
   printf("Starting the LWP...\n");
   // TODO: Get the first thread from the queue and start executing it
}


tid_t lwp_wait(int *status) {
   // ... your implementation ...
}

// Set the scheduler
void lwp_set_scheduler(scheduler newScheduler) {
   printf("Setting the scheduler...\n");
   activeScheduler = newScheduler;
   printf("Scheduler set successfully.\n");
}


scheduler lwp_get_scheduler(void) {
   // ... your implementation ...
}

thread tid2thread(tid_t tid) {
   // ... your implementation ...
}




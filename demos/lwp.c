// lwp.c
#include "lwp.h"
#include "rr.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>

// Create a new thread
tid_t lwp_create(lwpfun function, void *arg) {
   // Allocate memory for the new thread context
   thread new_thread = malloc(sizeof(context));
   void* new_thread_stack = mmap(NULL, sysconf(_SC_PAGE_SIZE), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

   // Check for memory allocation failures
   if (new_thread == NULL || new_thread_stack == MAP_FAILED) {
      return NO_THREAD;
   }

   new_thread->tid = ++last_tid;  // Assign a unique thread ID
   new_thread->stacksize = sizeof(sysconf(_SC_PAGE_SIZE)) * activeScheduler->qlen;
   new_thread->stack = new_thread_stack;
   new_thread->state.fxsave = FPU_INIT;

   // TODO: Initialize the registers in new_thread->state
   // TODO: Set up the initial stack frame to make sure the thread starts executing 'function'


   // Add the new thread to the queue
   activeScheduler->admit(new_thread);

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
   return activeScheduler;
   // ... your implementation ...
}

thread tid2thread(tid_t tid) {
   // ... your implementation ...
}




// lwp.c
#include "lwp.h"
#include "rr.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/resource.h>


// Create a new thread
#include <stdio.h>  // Ensure this is included for printf

tid_t lwp_create(lwpfun function, void *arg) {
   // Allocate memory for the new thread context
   thread new_thread = malloc(sizeof(context));

   size_t page_size = sysconf(_SC_PAGE_SIZE);
   printf("Got page_size: %zu.\n", page_size);  // Retained for stack info

   size_t stack_size;
   // Determine stack size
   struct rlimit rl;
   if (getrlimit(RLIMIT_STACK, &rl) == 0 && rl.rlim_cur != RLIM_INFINITY) {
      // Use the soft limit but ensure it's a multiple of page size
      stack_size = ((rl.rlim_cur + page_size - 1) / page_size) * page_size;
   } else {
      // Default to 8 MB
      stack_size = 8 * 1024 * 1024;
   }
   printf("Determined stack_size: %zu.\n", stack_size);  // Retained for stack info

   void* new_thread_stack = mmap(NULL, stack_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
   printf("Allocated new_thread_stack.\n");  // Retained for stack info

   // Check for memory allocation failures
   if (new_thread == NULL || new_thread_stack == MAP_FAILED) {
      return NO_THREAD;
   }

   new_thread->tid = ++last_tid;  // Assign a unique thread ID

   // Set the argument in the rdi register
   new_thread->state.rdi = (unsigned long)arg;

   // FPU Initialization
   new_thread->state.fxsave = FPU_INIT;

   // Stack Initialization
   new_thread->stacksize = stack_size;
   new_thread->stack = new_thread_stack;
   unsigned long *stack_start = (unsigned long *)(new_thread_stack + stack_size - sizeof(unsigned long));
   int offset = 0;

   stack_start[offset--] = (unsigned long)lwp_exit;   // Pushing exit function on the stack
   stack_start[offset--] = (unsigned long)function;   // Pushing function address on the stack

   // Move the base pointer to new base
   new_thread->state.rbp = (unsigned long)(stack_start + offset);

   // Push old base pointer address onto stack
   stack_start[offset--] = 0;

   // Set stack pointer to the top of the stack
   new_thread->state.rsp = (unsigned long)(stack_start + offset + 1);

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




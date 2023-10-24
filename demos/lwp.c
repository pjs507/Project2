// lwp.c
#include "lwp.h"
#include "rr.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/resource.h>

// Create a new thread

context *real_context = NULL;
context *current_thread = NULL;

tid_t lwp_create(lwpfun function, void *arg) {
   // Allocate memory for the new thread context
   thread new_thread = malloc(sizeof(context));

   size_t page_size = sysconf(_SC_PAGE_SIZE);

   size_t stack_size;
   // Determine stack size
   struct rlimit rl;
   if (getrlimit(RLIMIT_STACK, &rl) == 0 && rl.rlim_cur != RLIM_INFINITY) {
      stack_size = ((rl.rlim_cur + page_size - 1) / page_size) * page_size;
   } else {
      stack_size = 8 * 1024 * 1024;
   }
   void* new_thread_stack = mmap(NULL, stack_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);

   // Check for memory allocation failures
   if (new_thread == NULL || new_thread_stack == MAP_FAILED) {
      return NO_THREAD;
   }

   new_thread->tid = ++last_tid;

   // Set the argument in the rdi register
   new_thread->state.rdi = (unsigned long)arg;

   // FPU Initialization
   new_thread->state.fxsave = FPU_INIT;

   // Stack Initialization
   new_thread->stacksize = stack_size;
   new_thread->stack = new_thread_stack;

   unsigned long *stack_start = (unsigned long *)(new_thread_stack + stack_size - sizeof(unsigned long));
   int offset = 0;

   stack_start[offset--] = (unsigned long)lwp_exit;
   stack_start[offset--] = (unsigned long)function;


   // Move the base pointer to new base
   new_thread->state.rbp = (unsigned long)(stack_start + offset);

   // Push old base pointer address onto stack
   stack_start[offset--] = 0;

   // Set stack pointer to the top of the stack
   new_thread->state.rsp = (unsigned long)(stack_start + offset + 1);

   // Add the new thread to the queue
   activeScheduler->admit(new_thread);

   // Check if memory allocation was successful
   if(new_thread == NULL) {
      fprintf(stderr, "lwp_create(): Failed to allocate memory for new_thread.\n");
      return NO_THREAD;
   }
   if(new_thread_stack == MAP_FAILED) {
      fprintf(stderr, "lwp_create(): Failed to allocate memory for new_thread_stack.\n");
      return NO_THREAD;
   }

   return new_thread->tid;
}


// Exit the current thread
void lwp_exit(int status) {
   // TODO: Remove the current thread from the queue
   // TODO: Free resources
   // TODO: Switch to the next thread
}


tid_t lwp_gettid(void) {
   if (current_thread){
      return current_thread->tid;
   } else{
      return NO_THREAD;
   }
}

//TODO questions:
// What is the general flow of lwp_yield, and how can I be sure to loop back to the first processes, instead of running through them and Segfaulting after 5
// What are the uses of swap_rfiles and how are they different in lwp_start and lwp_yield? Are there other places it needs to swap?

void lwp_yield(void) {
   thread old = current_thread;
   current_thread = activeScheduler->next();

   if (current_thread){
      swap_rfiles(&(old->state), &(current_thread->state));
   }
   else{

   }
   //No threads left
   //exit(status);

}



// Start the scheduler
void lwp_start()
{
   fprintf(stderr, "Entering lwp_start()...\n"); // Entry point

   if(current_thread != NULL)
   {
      fprintf(stderr, "lwp_start(): An instance is already running...\n");
      return;
   }

   // Get a thread from the scheduler
   scheduler sched = lwp_get_scheduler();

   // Verify that a scheduler was found
   if(sched == NULL)
   {
      fprintf(stderr, "lwp_start(): Can't find a scheduler...\n");
      return;
   }

   current_thread = sched->next();

   // Check validity of next_thread
   if(current_thread == NULL)
   {
      fprintf(stderr, "lwp_start(): No more threads left in scheduler...\n");
      return;
   }
   else
   {
      fprintf(stderr, "next_thread address: %p\n", current_thread);
   }


   if(real_context == NULL)
   {
      fprintf(stderr, "real_context is NULL, allocating memory...\n"); // Before allocating memory for real_context

      // Allocate memory for the original context
      real_context = malloc(sizeof(context));
      if(real_context == NULL)
      {
         fprintf(stderr, "lwp_start(): Memory allocation failed.\n");
         return;
      }
      else
      {
         fprintf(stderr, "real_context address after allocation: %p\n", real_context);
      }
   }
   else
   {
      fprintf(stderr, "real_context address: %p\n", real_context);
   }


// Sanity check: Ensure pointers are not NULL
   if(real_context == NULL || current_thread == NULL) {
      fprintf(stderr, "Sanity check failed: One of the critical pointers is NULL.\n");
      return;
   }

   swap_rfiles(NULL, &(current_thread->state));


   free(real_context);
   real_context = NULL;
   current_thread = NULL;

}


tid_t lwp_wait(int *status) {
   // ... your implementation ...
}

void lwp_set_scheduler(scheduler newScheduler) {
   //TODO: add error handling
   activeScheduler = newScheduler;


}

scheduler lwp_get_scheduler(void) {
   return activeScheduler;
}

thread tid2thread(tid_t tid) {
   //TODO: get thread given some tid. Incorporate lib_one and lib_two?
}




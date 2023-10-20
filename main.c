#include <stdio.h>
#include <stdlib.h>
#include "lwp.h"
#include "scheduler.h"

#define MAX_THREADS 100


int main() {
   printf("Setting scheduler...\n");
   initQueue();
   printf("Starting LWP library...\n");
   lwp_start();
   return 0;
}

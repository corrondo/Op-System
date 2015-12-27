#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "AnR.h"

// The number of threads that will be created

const int NUM_THREADS = 100;

//mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// The maximum amount of time a thread can sleep for
const int MAX_SLEEP_TIME = 10;

//Create a function for the execution of the thread.

void *runner(void *param);

//Define the main function form where the execution of the thread begins.

int main(void){

// The pthread attributes

pthread_attr_t attr;

// An array for storing the thread ids

pthread_t thread_ids[NUM_THREADS];

// Used to index into the thread_ids array

int index;

// The value returned by functions

int ret;

// Allocate a map of pids

ret = allocate_map();

//Check the conditions whether ret is equal to 1 or not.

if (ret != 1) {
printf("allocate_map didn't work - %d", ret);

return 1;
}

// Initialize the random number generator

// with the current time

srand(time(NULL));

// Initialize pthreads

if (pthread_attr_init(&attr) != 0){

fputs("pthread_attr_init didn't work", stdout);

//Call the release_map function to release the map of pids.

release_map();

return 2;

}

// Loop NUM_THREADS times to create the threads

printf("Creating %d threads\n", NUM_THREADS);

for (index = 0; index < NUM_THREADS; index++){

// Create a thread

ret = pthread_create(&thread_ids[index], &attr, runner, NULL);

//Checking the condition whether ret is equal to 0 or not.

if (ret != 0){
printf("Couldn't create thread number %d", index + 1);

thread_ids[index] = -1;
}

}

//Iterate the Loop NUM_THREADS times to wait for the threads to exit.

for (index = 0; index < NUM_THREADS; index++)

// Check whether thread_ids[index] is greater than

// 0.

if (thread_ids[index] >= 0){

// Wait for a thread to exit

ret = pthread_join(thread_ids[index], NULL);

//Checking the condition whether ret is equal to 0 or not.

if (ret != 0)

printf("pthread_join didn't work for " "thread id %lx", thread_ids[index]);

}

//Define a method for De-initializing the pthreads

pthread_attr_destroy(&attr);

// Release the map of pids

release_map();

return 0;

}

//Define a function runner which takes one value as an arguments and it is a starting function for the threads.

void *runner(void *param){
	
/* acquire the mutex lock */
pthread_mutex_lock(&mutex);

/* critical section */

// Allocate a pid

int thread_id = allocate_pid();

printf("Allocated thread id %d \n", thread_id);

// Sleep a random amount of time

sleep(rand() % MAX_SLEEP_TIME + 1);

// Release the pid

release_pid(thread_id);

printf("Released thread id %d \n", thread_id);

/* release the mutex lock */
pthread_mutex_unlock(&mutex);

// Exit from the thread

pthread_exit(0);

}
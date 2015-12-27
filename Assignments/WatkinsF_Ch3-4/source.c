#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "AnR.h"



// Minimum value of pid

#define MIN_PID 300

// Maximum value of pid

#define MAX_PID 5000

// The number of bits at a position in the bitmap

#define NUM_BITS 8

// The bitmap of pids

static unsigned char *s_bitmap = NULL;

// The size of the bitmap

static int s_size = -1;

// The current index into the bitmap

static int s_index = -1;

// The current offset into the index of the bitmap

static int s_offset = -1;

// The last assigned pid

static int s_pid = -1;

//Allocate the map for the pids.

int allocate_map(void){

// Does the bitmap need to be allocated?

if (!s_bitmap){
// Calculate the size of the bitmap required

s_size = (int) ((double) (MAX_PID - MIN_PID + 1 + NUM_BITS - 1) / NUM_BITS);

// Allocate the bitmap

s_bitmap = (__typeof__(s_bitmap)) malloc(s_size);}

if (s_bitmap){

// Initialize the variables

s_index = s_offset = 0;

s_pid = MIN_PID - 1;

return 1;

} else

return -1;

}

//Release the bitmap

void release_map(void){

// Does the bitmap exist?

if (s_bitmap){

// Free it & deinitialize the variables

free(s_bitmap);

s_bitmap = NULL;

s_index = s_offset = s_pid = -1;

}

}

//Allocate a pid in a range. It assumes that bitmap is valid. 
//While loop is used to execute the condition until pindex is less then size.

static int allocate_pid_range(int *pindex, const int size){

// While the index is less than the size

while (*pindex < size){

// While the offset within an element

// is less than the number of bits

while (s_offset < NUM_BITS){

// Increment the pid

s_pid++;

// Did the pid exceed the maximum?

if (s_pid > MAX_PID){

// Reset the pid and the offset

s_pid = MIN_PID - 1;

s_offset = 0;

return -1;
}

// Is the offset free?

if (!(s_bitmap[*pindex] & (1u << s_offset))){

// Fill the offset

s_bitmap[*pindex] |= 1u << s_offset;

// Increment the offset

s_offset++;

return 0;
}

// Increment the offset

s_offset++;
}

// Reset the offset

s_offset = 0;

// Increment the index

(*pindex)++;
}

return -1;
}

//Allocate a pid.

int allocate_pid(void){

// Does the bitmap exist?

if (s_bitmap){

// Try to allocate a pid from the current index to the size of the bitmap

int index = s_index;

int ret = allocate_pid_range(&index, s_size);

// Could a pid be allocated?

if (ret < 0){

// Reset the index and try again

index = 0;

ret = allocate_pid_range(&index, s_index);
}

// Was a pid successfully allocated?

if (ret == 0){

// Update the index

s_index = index;
// Return the pid
return s_pid;
}
}

return -1;
}

//Release an allocated pid.

void release_pid(int pid){

// Does the bitmap exist and is the pid valid?

if (s_bitmap && pid >= MIN_PID && pid <= MAX_PID){

// Subtract MIN_PID so it can be

// used to access the bitmap

pid -= MIN_PID;

// Clear the entry for the pid in the bitmap

s_bitmap[pid / NUM_BITS]

&= ~(1u << (pid % NUM_BITS));
}
}
    /***********************************************
****               Managing the process id.         ****
*  Unique process identifier for each and every process.*
*********************************************************/



#ifndef AnR_H

#define AnR_H

// Allocate the bitmap

extern int allocate_map(void);

// Release the bitmap

extern void release_map(void);

// Allocate a pid

extern int allocate_pid(void);

// Release a pid

extern void release_pid(int pid);

#endif
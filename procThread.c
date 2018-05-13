#include "types.h"
#include "user.h"
#include "procThread.h"

int procThread_create(void)
{
 void* stack = malloc(4096); 
int tid = clone((void *)stack);
//free(stack);
//printf(1,"Thread id- %d",tid);
return tid;
    
}


void procThread_exit(int ret_val)
{
    thread_exit(ret_val);
    
}


void procThread_join(int tid, int * ret_val_p)
{
    void* stack;
    join(tid,ret_val_p,&stack);
    //free(stack);
    
}


#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

extern int free_frame_cnt; // CS550

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
int sys_shutdown(void)
{
  /* Either of the following will work. Does not harm to put them together. */
  outw(0xB004, 0x0|0x2000); // working for old qemu
  outw(0x604, 0x0|0x2000); // working for newer qemu

  return  0;
}
int sys_print_free_frame_cnt(void)
{
    cprintf("free-frames %d\n", free_frame_cnt);
    return 0;
}
int sys_clone(void)
{
  void * ptr;
  argptr(0, (char **) &ptr, sizeof(void*));
  return clone(ptr);
}
int sys_join(void) 
{
  //Defining parameters
  int * ret_p;
  int tid;
  void** stack;
  
  //Validating Inputs  
  if (argint(0,&tid) < 0)
	return -1;
  if (argptr(1,(char **)&ret_p,sizeof(int*)) < 0)
	return -1;
  if(argptr(2, (void*)&stack, sizeof(void **)) < 0){
	return -1;
    }
  join(tid,ret_p,stack);
return 1;
}

//Project 3: It does the validations (input parameters)
//and calls the thread_exit function
void
sys_thread_exit(void) 
{
  //Defining parameters
  int ret_val_p;
  
  //Validating Inputs
  if (argint(0,&ret_val_p) < 0) {
	return;
   }
  thread_exit(ret_val_p);
}
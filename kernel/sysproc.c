#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "vm.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  kexit(n);
  return 0; // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return kfork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return kwait(p);
}

uint64
sys_wait2(void)
{
  uint64 status;
  uint64 rusage;

  argaddr(0, &status);
  argaddr(1, &rusage);

  return kwait2(status, rusage);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int t;
  int n;

  argint(0, &n);
  argint(1, &t);
  addr = myproc()->sz;

  if (t == SBRK_EAGER || n < 0) {
    if (growproc(n) < 0) {
      return -1;
    }
  } else {
    // Lazily allocate memory for this process: increase its memory
    // size but don't allocate memory. If the processes uses the
    // memory, vmfault() will allocate it.
    if (addr + n < addr)
      return -1;
    if (addr + n > TRAPFRAME)
      return -1;
    myproc()->sz += n;
  }
  return addr;
}

uint64
sys_pause(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if (n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n) {
    if (killed(myproc())) {
      release(&tickslock);
      return -1;
    }
    sleep_prepare(&ticks);
    release(&tickslock);
    sleep();
    acquire(&tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kkill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

//get information about the processes
uint64
sys_getprocs(void)
{
  //store the address of the process array
  uint64 addr;
  //get the address passed by the user
  argaddr(0, &addr);
  //get the process information
  return getprocs(addr);
}

//get the current process priority
uint64
sys_getpriority(void)
{
  struct proc *p = myproc();
  int priority;
  //lock the process
  acquire(&p->lock);
  //get the priority
  priority = p->priority;
  release(&p->lock);
  return priority;
}

//set the current process priority
uint64
sys_setpriority(void)
{
  struct proc *p = myproc();
  int priority;
  //get the new priority
  argint(0, &priority);
  //check if the priority is valid
  if(priority < 0 || priority > 49){
    return -1;
  }
  //lock the process
  acquire(&p->lock);
  //update the priority
  p->priority = priority;
  release(&p->lock);
  return 0;
}

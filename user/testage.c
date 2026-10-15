#include "kernel/types.h"
#include "kernel/pstat.h"
#include "kernel/param.h"
#include "user/user.h"

int
main(void)
{
  int pids[5];
  struct pstat procs[NPROC];
  uint maxage = 0;

  //create five child processes
  for(int i = 0; i < 5; i++){
    pids[i] = fork();
    if(pids[i] < 0){
      printf("fork failed\n");
      exit(1);
    }
    if(pids[i] == 0){
      //keep the child busy
      while(1){
      }
    }
  }

  //check how long the children wait
  for(int i = 0; i < 100000; i++){
    int count = getprocs(procs);
    for(int j = 0; j < count; j++){
      for(int k = 0; k < 5; k++){
        if(procs[j].pid == pids[k] && procs[j].state == 3){
          if(procs[j].age > maxage){
            maxage = procs[j].age;
          }
        }
      }
    }
  }

  printf("maximum child age: %d ticks\n", maxage);

  //stop all child processes
  for(int i = 0; i < 5; i++){
    kill(pids[i]);
  }
  for(int i = 0; i < 5; i++){
    wait(0);
  }
  exit(0);
}

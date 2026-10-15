#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
  int pids[3];
  int priorities[3] = {5, 20, 40};

  //create three processes with different priorities
  for(int i = 0; i < 3; i++){
    pids[i] = fork();
    if(pids[i] < 0){
      printf("fork failed\n");
      exit(1);
    }
    if(pids[i] == 0){
      setpriority(priorities[i]);
      //run some work
      volatile int result = 0;
      for(int j = 0; j < 1000000; j++){
        result++;
      }
      printf("child priority %d finished\n", getpriority());
      exit(0);
    }
  }

  //wait for all children
  for(int i = 0; i < 3; i++){
    wait(0);
  }
  exit(0);
}

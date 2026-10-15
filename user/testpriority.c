#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
  //get the starting priority
  printf("starting priority: %d\n", getpriority());
  //change the priority to 10
  if(setpriority(10) < 0){
    printf("could not change priority\n");
    exit(1);
  }
  //check the new priority
  printf("new priority: %d\n", getpriority());
  //test an invalid priority
  printf("setting priority to 60: %d\n", setpriority(60));
  //check that the priority stayed the same
  printf("final priority: %d\n", getpriority());
  //create a child process
  int pid = fork();
  if(pid == 0){
    //check the child's priority
    printf("child priority: %d\n", getpriority());
    exit(0);
  }
  else if(pid > 0){
    //wait for the child
    wait(0);
  }
  else{
    printf("fork failed\n");
  }
  exit(0);
}

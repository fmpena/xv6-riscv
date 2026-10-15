
#include "kernel/types.h"
#include "user/user.h"

struct result {
  int priority;
  int response;
  int turnaround;
};

int
main(void)
{
  int ready[2];
  int start[2];
  int results[2];
  int priorities[3] = {5, 20, 40};
  int pids[3];
  struct result data;
  int totalresponse = 0;
  int totalturnaround = 0;

  if(pipe(ready) < 0 || pipe(start) < 0 || pipe(results) < 0){
    printf("pipe failed\n");
    exit(1);
  }

  //create three processes
  for(int i = 0; i < 3; i++){
    pids[i] = fork();
    if(pids[i] < 0){
      printf("fork failed\n");
      exit(1);
    }
    if(pids[i] == 0){
      close(ready[0]);
      close(start[1]);
      close(results[0]);
      setpriority(priorities[i]);

      //tell the parent this process is ready
      char signal = 'R';
      write(ready[1], &signal, 1);
      read(start[0], &signal, 1);

      //record when the process starts working
      int first = uptime();
      volatile int work = 0;
      for(int j = 0; j < 10000000; j++){
        work++;
      }
      int finished = uptime();

      data.priority = priorities[i];
      data.response = first;
      data.turnaround = finished;
      write(results[1], &data, sizeof(data));
      exit(0);
    }
  }

  close(ready[1]);
  close(start[0]);
  close(results[1]);

  //wait until all children are ready
  char signal;
  for(int i = 0; i < 3; i++){
    read(ready[0], &signal, 1);
  }

  //release all children
  int beginning = uptime();
  for(int i = 0; i < 3; i++){
    signal = 'S';
    write(start[1], &signal, 1);
  }

  //collect the results
  for(int i = 0; i < 3; i++){
    read(results[0], &data, sizeof(data));
    int response = data.response - beginning;
    int turnaround = data.turnaround - beginning;
    printf("priority %d: response %d, turnaround %d\n",
           data.priority, response, turnaround);
    totalresponse += response;
    totalturnaround += turnaround;
  }

  printf("average response: %d ticks\n", totalresponse / 3);
  printf("average turnaround: %d ticks\n", totalturnaround / 3);

  for(int i = 0; i < 3; i++){
    wait(0);
  }
  close(ready[0]);
  close(start[1]);
  close(results[0]);
  exit(0);
}

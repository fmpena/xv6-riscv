struct rusage {
  uint cputime;
};

struct pstat {
    int pid;
    int state;
    uint64 size;
    int ppid;
    int priority; //store the process priority
    uint age; //time spent waiting to run
    char name[16];
};

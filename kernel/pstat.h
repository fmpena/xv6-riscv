struct rusage {
  uint cputime;
};

struct pstat {
    int pid;
    int state;
    uint64 size;
    int ppid;
    int priority; //store the process priority
    char name[16];
};

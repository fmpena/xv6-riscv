struct rusage {
  uint cputime;
};

struct pstat {
    int pid;
    int state;
    uint64 size;
    int ppid;
    char name[16];
};

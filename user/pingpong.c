#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int pid, p2c[2], c2p[2]; char buf = 'a';

  if(pipe(p2c) < 0 || pipe(c2p) < 0){
    fprintf(2, "pipe: cannot create pipe\n");
    exit(1);
  }
  if((pid = fork()) < 0){
    fprintf(2, "fork: cannot fork process\n");
    exit(1);
  }
  if(pid > 0){  // parent
    write(p2c[1], &buf, 1);
    read(c2p[0], &buf, 1);
    printf("%d: received pong\n", getpid());
  } else {      // child
    read(p2c[0], &buf, 1);
    printf("%d: received ping\n", getpid());
    write(c2p[1], &buf, 1);
  }
  exit(0);
}

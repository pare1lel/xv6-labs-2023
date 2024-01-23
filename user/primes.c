#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
sieve(int pl[2])
{
  int pr[2], num, pid, buf;

  close(pl[1]);
  if(read(pl[0], &num, sizeof(num)) <= 0)
    exit(0);
  printf("prime %d\n", num);
  if(pipe(pr) < 0){
    fprintf(2, "pipe: cannot create pipe\n");
    exit(1);
  }
  if((pid = fork()) < 0){
    fprintf(2, "fork: cannot fork process\n");
    exit(1);
  }
  if(pid > 0){  // parent
    close(pr[0]);
    while(read(pl[0], &buf, sizeof(buf)) > 0)
      if(buf % num)
        write(pr[1], &buf, sizeof(buf));
    close(pl[0]);
    close(pr[1]);
    wait(0);
  } else {      // child
    close(pl[0]);
    sieve(pr);
  }
}

int
main(int argc, char *argv[])
{
  int p[2], i;

  if(pipe(p) < 0){
    fprintf(2, "pipe: cannot create pipe\n");
    exit(1);
  }
  for(i = 2; i <= 35; ++i)
    write(p[1], &i, sizeof(i));
  sieve(p);
  exit(0);
}

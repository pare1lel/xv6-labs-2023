#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

int
main(int argc, char *argv[])
{
  if(argc < 2){
    fprintf(2, "usage: xargs <command>\n");
    exit(1);
  }
  if(argc >= MAXARG){
    fprintf(2, "xargs: too many arguments\n");
    exit(1);
  }

  char *cmd = argv[1], *params[MAXARG], buf[512];
  int i, pid;

  for(i = 0; i + 1 < argc; ++i)
    params[i] = argv[i + 1];
  params[argc] = 0;
  for(i = 0; read(0, buf + i, 1); ++i)
    if(buf[i] == '\n'){
      buf[i] = 0;
      params[argc - 1] = buf;
      if((pid = fork()) < 0){
        fprintf(2, "fork: cannot fork process\n");
        exit(1);
      }
      if(pid > 0)
        wait(0);
      else if(exec(cmd, params) < 0){
        fprintf(2, "exec: cannot exec %s\n", cmd);
        exit(1);
      }
    }
  exit(0);
}

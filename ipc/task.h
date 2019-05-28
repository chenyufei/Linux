#ifndef TASK_H
#define TASK_H


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <dirent.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/ipc.h>


extern int getPidByName(pid_t *pid, char *task_name);

extern int getNameByPid(pid_t pid, char *task_name);


#endif

#ifndef _COMMANDS_H
#define _COMMANDS_H
#include <unistd.h> 
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "JobsAndInfo.h"

#define MAX_LINE_SIZE 80
#define MAX_ARG 20
int ExeComp(char* lineSize);
int BgCmd(char* lineSize, void* jobs);
int ExeCmd(void* jobs, char* lineSize, char* cmdString, char* LastPath, CmdHistory* hist);
void ExeExternal(char *args[MAX_ARG], char* cmdString, CmdHistory* hist);
#endif


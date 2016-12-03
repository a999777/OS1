#ifndef _SIGS_H
#define _SIGS_H
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <signal.h>
#include "JobsAndInfo.h"

void handle_CTRL_z(int sig_num);
void handle_CTRL_c(int sig_num);

#endif


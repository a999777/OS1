// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.h"

extern JobsVect* jobs;//Global

//SIGTSTP
//**************************************************************************************
// function name: Ctrl_z_handler
// Description: handles the Ctrl-z signal - if process runs in fg - suspend it and add to joblist
// Parameters: signum - signal number 
// Returns: 
//**************************************************************************************
void handle_CTRL_z(int sig_num) {
	//assert(sig_num >= 0);//Make sure TODO
	if (jobs->isEmpty() == true) {// no processes in fg 
		return;
	}
	else if (kill(jobs->newestJobPid(), SIGTSTP) == 0) {// 0 is successful
		//TODO
	} else {
		//TODO
	}
	return;
}

//SIGINT
//**************************************************************************************
// function name: Ctrl_c_handler
// Description: handles the Ctrl-c signal - terminates process if in fg , does nothing otherwise
// Parameters: signum - signal number 
// Returns: 
//**************************************************************************************
void handle_CTRL_c(int sig_num) {
	return;
}

//SIGSTOP sent by kill(), can't be intercepted by process
//SIGCONT un-suspends the process (counterpart to SIGTSTP & SIGSTOP)

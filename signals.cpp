// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.h"

//SIGTSTP
//**************************************************************************************
// function name: Ctrl_z_handler
// Description: handles the Ctrl-z signal - if process runs in fg - suspend it and add to joblist
// Parameters: signum - signal number 
// Returns: 
//**************************************************************************************
void handle_CTRL_z(int sig_num) {
	assert(sig_num >= 0);//Make sure
	
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

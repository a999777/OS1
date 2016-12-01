// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.h"

extern int fgProcessID;//Global FIXME
extern char fgCmdName[MAX_LINE_SIZE];//Global FIXME

//SIGTSTP
//**************************************************************************************
// function name: Ctrl_z_handler
// Description: handles the Ctrl-z signal - if process runs in fg - suspend it and add to joblist
// Parameters: signum - signal number 
// Returns: 
//**************************************************************************************
void handle_CTRL_z(int sig_num) {
	//assert(sig_num >= 0);//Make sure TODO
	if (fgProcessID == NO_PROCESS_RUNNING) {// no processes in fg 
		return;
	}
	else if (kill(fgProcessID, SIGTSTP) == 0) {// 0 is successful
		InsertJob(JobsArray, L_Fg_Cmd, GPid, 1); // insert fg process to jobarrary with susp flag
		// do we need to treat the case a signak is sent to proccess in bg??
		printf("signal SIGTSTP was sent to pid %d\n", GPid);
		GPid = -1; // now no process in fg
		L_Fg_Cmd[0] = '\0'; 
	} else {
		printf("smash error: > cannot send signal SIGTSTP to pid %d\n",GPid); // if could not send signal
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

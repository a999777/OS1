// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.h"

extern JobsVect* jobs;		//Global
extern string globalCmdName;//Global
extern int globalCmdPID;	//Global

//SIGTSTP
//**************************************************************************************
// function name: handle_CTRL_z
// Description: handles the CTRL+Z signal(SIGTSTP) - if a process runs in fg -
// we will suspend it and add to joblist
// Parameters: sig_num - signal number 
//**************************************************************************************
void handle_CTRL_z(int sig_num) {
	if (globalCmdPID == NO_PROCESS_RUNNING) {// no processes in fg 
		cout << " CTRL+Z: no processes in fg" << endl;
		return;
	}
	else if (kill(globalCmdPID, SIGTSTP) == KILL_SUCCESS) {
		jobs->insertJob(globalCmdName, globalCmdPID, JOB_WAS_SUSPENDED);
		cout << "smash > signal SIGTSTP was sent to pid " << globalCmdPID << endl;
		globalCmdPID = NO_PROCESS_RUNNING;
	} else {
		cout << "CTRL+Z: kill did not work" << endl;
	}
	return;
}

//SIGINT
//**************************************************************************************
// function name: handle_CTRL_c
// Description: handles the CTRL+C signal(SIGINT) - terminates a process if in fg
// Parameters: sig_num - signal number 
//**************************************************************************************
void handle_CTRL_c(int sig_num) {
	if (globalCmdPID == NO_PROCESS_RUNNING) {// no processes in fg 
		cout << " CTRL+C: no processes in fg" << endl;
		return;
	}
	else if (kill(globalCmdPID, SIGINT) == KILL_SUCCESS) {
		jobs->insertJob(globalCmdName, globalCmdPID, JOB_WAS_SUSPENDED);
		cout << "smash > signal SIGINT was sent to pid " << globalCmdPID << endl;
		globalCmdPID = NO_PROCESS_RUNNING;
	} else {
		cout << "CTRL+C: kill did not work" << endl;
	}
	return;
}

//SIGSTOP sent by kill(), can't be intercepted by process
//SIGCONT un-suspends the process (counterpart to SIGTSTP & SIGSTOP)

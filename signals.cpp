// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.h"

extern JobsVect* jobs;//Global
extern string globalCmdName;//Global
extern int globalCmdPID;//Global

//SIGTSTP
//**************************************************************************************
// function name: handle_CTRL_z
// Description: handles the CTRL+Z signal - if process runs in fg - suspend it and add to joblist
// Parameters: sig_num - signal number 
//**************************************************************************************
void handle_CTRL_z(int sig_num) {
	//assert(sig_num >= 0);//Make sure TODO
	if (globalCmdPID == NO_PROCESS_RUNNING) {// no processes in fg 
		cout << "CTRL+Z: no processes in fg" << endl; //TODO make better
		return;
	}
	else if (kill(globalCmdPID, SIGTSTP) == KILL_SUCCESS) {
		jobs->insertJob(globalCmdName, globalCmdPID, JOB_WAS_SUSPENDED);
		cout << "CTRL+Z: killed process" << endl; //TODO make better
		cout << "signal SIGTSTP was sent to pid " << globalCmdPID << endl;
		globalCmdPID = NO_PROCESS_RUNNING;
	} else {
		cout << "CTRL+Z: kill did not work" << endl; //TODO make better
	}
	return;
}

//SIGINT
//**************************************************************************************
// function name: handle_CTRL_c
// Description: handles the CTRL+C signal - terminates process if in fg , does nothing otherwise
// Parameters: sig_num - signal number 
//**************************************************************************************
void handle_CTRL_c(int sig_num) {
	//assert(sig_num >= 0);//Make sure TODO
	if (globalCmdPID == NO_PROCESS_RUNNING) {// no processes in fg 
		cout << "CTRL+C: no processes in fg" << endl; //TODO make better
		return;
	}
	else if (kill(globalCmdPID, SIGINT) == KILL_SUCCESS) {
		jobs->insertJob(globalCmdName, globalCmdPID, JOB_WAS_SUSPENDED);
		cout << "CTRL+C: killed process" << endl; //TODO make better
		cout << "signal SIGINT was sent to pid " << globalCmdPID << endl;
		globalCmdPID = NO_PROCESS_RUNNING;
	} else {
		cout << "CTRL+C: kill did not work" << endl; //TODO make better
	}
	return;
}

//SIGSTOP sent by kill(), can't be intercepted by process
//SIGCONT un-suspends the process (counterpart to SIGTSTP & SIGSTOP)

/*	smash.c
main file. This file contains the main function of smash
*******************************************************************/
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <iostream>
#include <cstdlib>
#include <cstring>
#include "commands.h"
#include "signals.h"
#include "JobsAndInfo.h"

JobsVect* jobs;				//Global
string globalCmdName;		//Global
int globalCmdPID;			//Global

#define MAX_LINE_SIZE 80
#define MAXARGS 20

char lineSize[MAX_LINE_SIZE]; 

//**************************************************************************************
// function name: main
// Description: main function of smash. get command from user and calls command function
//**************************************************************************************
int main(int argc, char *argv[])
{
    char cmdString[MAX_LINE_SIZE]; 	   
	
	//Signal declarations. We chose to implement using sigaction which means
    //we need to use the sigaction struct.
    struct sigaction Ctrl_C,Ctrl_Z;
    Ctrl_C.sa_handler = &handle_CTRL_c;
    Ctrl_Z.sa_handler = &handle_CTRL_z;
	sigaction(SIGINT, &Ctrl_C, NULL);
	sigaction(SIGTSTP, &Ctrl_Z, NULL);

	/************************************/

	// Init variables that goes to other modules.
    CmdHistory* hist = new CmdHistory();
    char LastPath[MAX_LINE_SIZE];
    jobs = new JobsVect();
    globalCmdPID = NO_PROCESS_RUNNING;

    /************************************/

    //Heart of the program- the loop that catches and classifies the commands.
    while (1)
    {
		//FIXME these extra 2 lines fix the bug for some unknown reason???
		sigaction(SIGINT, &Ctrl_C, NULL);//FIXME testing
		sigaction(SIGTSTP, &Ctrl_Z, NULL);//FIXME testing
    	printf("smash > ");
		fgets(lineSize, MAX_LINE_SIZE, stdin);
		strcpy(cmdString, lineSize);    	
		cmdString[strlen(lineSize)-1]='\0';
		// perform a complicated Command
		if(!ExeComp(lineSize,hist)) continue;
		// background command
	 	if(!BgCmd(lineSize, hist, jobs)) continue;
	 	// built in commands
		ExeCmd(lineSize, cmdString, LastPath, hist);
		/* initialize for next line read*/
		lineSize[0]='\0';
		cmdString[0]='\0';
	}
    return 0;
}


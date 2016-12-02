/*	smash.c
main file. This file contains the main function of smash
*******************************************************************/
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "commands.h"
#include "signals.h"
#include "JobsAndInfo.h"

JobsVect* jobs;//Global
char globalCmdName[MAX_LINE_SIZE];//Global
int globalCmdPID;//Global


#define MAX_LINE_SIZE 80
#define MAXARGS 20



char* L_Fg_Cmd;

char lineSize[MAX_LINE_SIZE]; 


//**************************************************************************************
// function name: main
// Description: main function of smash. get command from user and calls command functions
//**************************************************************************************
int main(int argc, char *argv[])
{
    char cmdString[MAX_LINE_SIZE]; 	   
	
	//signal declaretions
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	 /* add your code here */

	
	/************************************/
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	//set your signal handlers here
	/* add your code here */

	/************************************/

	/************************************/
	// Init globals 

    CmdHistory* hist = new CmdHistory();
    char LastPath[MAX_LINE_SIZE];
    jobs = new JobsVect();
    globalCmdPID = NO_PROCESS_RUNNING;
    globalCmdName[0] = '\0'; 
    //fgProcessID = NO_PROCESS_RUNNING;//marking fg proccess running FIXME
	//fgCmdName[0] = '\0'; FIXME
	L_Fg_Cmd =(char*)malloc(sizeof(char)*(MAX_LINE_SIZE+1));
	if (L_Fg_Cmd == NULL) 
			exit (-1); 
	L_Fg_Cmd[0] = '\0';
    	while (1)
    	{
	 	printf("smash > ");
		fgets(lineSize, MAX_LINE_SIZE, stdin);
		strcpy(cmdString, lineSize);    	
		cmdString[strlen(lineSize)-1]='\0';
		//strcpy(fgCmdName, lineSize); FIXME
		//fgCmdName[strlen(lineSize)-1]='\0'; FIXME
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


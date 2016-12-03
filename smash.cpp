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

void printNewLineForTests (char* lineSize, char* cmdString, char* LastPath, CmdHistory* hist);//FIXME for script testing
bool isTesting = false;// FIXME for testing


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

    //globalCmdName[0] = '\0'; //TODO Eitan can that be removed?

    /************************************/

    //Heart of the program- the loop that catches and classifies the commands.
    while (1)
    {
    	printf("smash > ");
    	//cout << "smash > ";//TODO maby
		fgets(lineSize, MAX_LINE_SIZE, stdin);
		printNewLineForTests(lineSize, cmdString, LastPath, hist);//FIXME for script testing
		/*if (isTesting == true) {
			printNewLineForTests(lineSize, cmdString, LastPath, hist);//FIXME for script testing
		}*/
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



void printNewLineForTests (char* lineSize, char* cmdString, char* LastPath, CmdHistory* hist) {//FIXME for script testing
	if (isTesting == false) {
		return;
	}
	bool shouldIPrint = false;
	char* cmd;
	char* args[MAX_ARG];
	char pwd[MAX_LINE_SIZE];
	const char* delimiters = " \t\n";
	int i = 0, num_arg = 0;
	bool illegal_cmd = false; // illegal command
    cmd = strtok(lineSize, delimiters);
	if (cmd == NULL) {
		return;
	}
   	args[0] = cmd;
	for (i = 1; i < MAX_ARG; i++)
	{
		args[i] = strtok(NULL, delimiters);
		if (args[i] != NULL)
			num_arg++;

	}
    if (0) {

    }
	else if (!strcmp(cmd, "history"))
	{
 		if(num_arg != 0) {
 			illegal_cmd = true;
 		} else {
 			shouldIPrint = true;
 		}
	}
	else if (!strcmp(cmd, "jobs"))
	{
		if(num_arg != 0) {
			illegal_cmd = true;
		} else {
			//get jobs list
			shouldIPrint = true;
			if (jobs->isEmpty() == true) {
				shouldIPrint = true;
			}
		}
	}
	else if (!strcmp(cmd, "quit"))
	{
   		if (num_arg < 0 || num_arg > 1) {//Validate number of arguments
   			illegal_cmd = true;
   		} else if (num_arg == 0) { //normal quit
   			shouldIPrint = true;
   		} else if (!(strcmp(args[1], "kill")) == false) { //there's an argument, but it's not kill
   			illegal_cmd = true;
   		} else { //quit kill
   			shouldIPrint = true;
			}
	}
	else if (!strcmp(cmd, "cd")) //FIXME not good enough according to faq, look at the command error
	{
		int ChangeDirRes = ERROR_VALUE;
		//verifying only 1 argument. An error should be print as illegal cmd
		if (num_arg != 1) {
			illegal_cmd = true;
		} else if (!strcmp(args[1],"-")) {				//change to last dir
			if (LastPath == NULL) {
				illegal_cmd = true;
			} else {
				ChangeDirRes = chdir(LastPath);
				if (ChangeDirRes == ERROR_VALUE){ 		//Error occured

				} else {						//Directory change succeeded
					shouldIPrint = true;
				}
			}

		} else {								//Changing to a new path
			ChangeDirRes = chdir(args[1]);
			if (ChangeDirRes == ERROR_VALUE){ 		//Error occured while switching

			} else {						//Directory change succeeded
				shouldIPrint = true;
			}
		}
	}
	else if (!strcmp(cmd, "pwd"))
	{
		if(num_arg != 0) {						//verifying no arguments
			illegal_cmd = true;
		} else if(!getcwd(pwd, MAX_LINE_SIZE)) { //Null means getcwd failed

		} else {								 //Meaning getcwd succeeded
			shouldIPrint = true;
		}
	}



	if (illegal_cmd == false && shouldIPrint == true) {
    	cout << cmdString;
    	cout << lineSize;
    	cout << endl;//FIXME  for script testing
    }

}

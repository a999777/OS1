//		commands.c
//********************************************
#include "commands.h"
#include <unistd.h>
#include <iostream>
#include <string>
#include <cstring>

using std::cout;
using std::endl;
using std::string;

//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
int ExeCmd(void* jobs, char* lineSize, char* cmdString, char* LastPath, CmdHistory* hist)
{
	char* cmd; 
	char* args[MAX_ARG];
	char pwd[MAX_LINE_SIZE];
	const char* delimiters = " \t\n";
	int i = 0, num_arg = 0;
	bool illegal_cmd = false; // illegal command
    	cmd = strtok(lineSize, delimiters);
	if (cmd == NULL)
		return 0; 
   	args[0] = cmd;
	for (i=1; i<MAX_ARG; i++)
	{
		args[i] = strtok(NULL, delimiters); 
		if (args[i] != NULL) 
			num_arg++; 
 
	}
/*************************************************/
// Built in Commands PLEASE NOTE NOT ALL REQUIRED
// ARE IN THIS CHAIN OF IF COMMANDS. PLEASE ADD
// MORE IF STATEMENTS AS REQUIRED
/*************************************************/

	if (!strcmp(cmd, "cd") ) 
	{
		int ChangeDirRes = -1;
		if(!getcwd(pwd,MAX_LINE_SIZE)) {
			perror("getcwd error");
		}

		//verifying only 1 argument. An error should be print as illegal cmd
		if(num_arg != 1) {
			illegal_cmd = true;

		} else if(!strcmp(args[1],"-")) {				//change to last dir
			if(LastPath == NULL) {
				illegal_cmd = true;
			} else {
				ChangeDirRes = chdir(LastPath);
				if(ChangeDirRes == -1){ 		//Error occured
					perror("chdir error");		//TODO The pdf says we should do a specific error.
				} else {						//Directory change succeeded
					cout << LastPath << endl;
					strcpy(LastPath,pwd);
					hist->addString(string("cd -"));
				}
			}

		} else {								//Changing to a new path
			ChangeDirRes = chdir(args[1]);
			if(ChangeDirRes == -1){ 		//Error occured while switching
				perror("chdir error");		//TODO
			} else {						//Directory change succeeded
				strcpy(LastPath,pwd);
				cout << args[1] << endl;
				string savedCmd = string("cd ");
				savedCmd.append(args[1]);
				hist->addString(savedCmd);
			}
		}
	} 
	
	/*************************************************/
	else if (!strcmp(cmd, "pwd")) 
	{
		if(num_arg != 0) {						//verifying no arguments
			illegal_cmd = true;
		} else if(!getcwd(pwd, MAX_LINE_SIZE)) { //Null means getcwd failed
			perror("getcwd error");
		} else {								 //Meaning getcwd succeeded
			cout << pwd << endl;
			hist->addString(string("pwd"));
		}
	}
	
	/*************************************************/
	else if (!strcmp(cmd, "history"))
	{
 		if(num_arg != 0) {
 			illegal_cmd = true;
 		} else {
 			hist->printAll();
 			hist->addString(string("history"));
 		}
	}
	/*************************************************/
	
	else if (!strcmp(cmd, "jobs")) 
	{
 		
	}
	/*************************************************/
	else if (!strcmp(cmd, "showpid")) 
	{
		
	}
	/*************************************************/
	else if (!strcmp(cmd, "fg")) 
	{
		
	} 
	/*************************************************/
	else if (!strcmp(cmd, "bg")) 
	{
  		
	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
   		
	} 
	/*************************************************/
	else // external command
	{
 		ExeExternal(args, cmdString, hist);
	 	return 0;
	}
	if (illegal_cmd == true)
	{
		printf("smash error: > \"%s\"\n", cmdString);
		return 1;
	}
    return 0;
}
//**************************************************************************************
// function name: ExeExternal
// Description: executes external command
// Parameters: external command arguments, external command string
// Returns: void
//**************************************************************************************
void ExeExternal(char *args[MAX_ARG], char* cmdString, CmdHistory* hist)
{
	int pID, status;
    	switch(pID = fork()) 
	{
    		case -1: 
				//Error of "fork"
    			perror("Failed to Create Child Process");
    			exit(1); //Only father can run this (and die)
        	case 0 :
                // Child Process. Changing the group id.
               	setpgrp();
			    // Execute an external command.
               	execvp(args[0],args);
				//If we got here that means execvp failed.
               	perror("Failed to execute external command");
               	exit(1);
			default:
				//Father process. Saves the id of the child and wait for it to end
				int ChildpID = pID;
				waitpid(ChildpID, &status, WUNTRACED);
				hist->addString(string(*args)); //TODO History does not show the arguments of the external command. Fix it
				break;
	}
}
//**************************************************************************************
// function name: ExeComp
// Description: executes complicated command
// Parameters: command string
// Returns: 0- if complicated -1- if not
//**************************************************************************************
int ExeComp(char* lineSize)
{
	char ExtCmd[MAX_LINE_SIZE+2];
	char *args[MAX_ARG];
    if ((strstr(lineSize, "|")) || (strstr(lineSize, "<")) || (strstr(lineSize, ">")) || (strstr(lineSize, "*")) || (strstr(lineSize, "?")) || (strstr(lineSize, ">>")) || (strstr(lineSize, "|&")))
    {
		// Add your code here (execute a complicated command)
					
		/* 
		your code
		*/
	} 
	return -1;
}
//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
int BgCmd(char* lineSize, void* jobs)
{

	char* Command;
	const char* delimiters = " \t\n";
	char *args[MAX_ARG];
	if (lineSize[strlen(lineSize)-2] == '&')
	{
		lineSize[strlen(lineSize)-2] = '\0';
		// Add your code here (execute a in the background)
					
		/* 
		your code
		*/
		
	}
	return -1;
}


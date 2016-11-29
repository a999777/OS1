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

#define PRINT_JOB(a, b , c, d) \
	cout << "[" << a << "]" << b << " : " << c << " " << d << " secs" << endl; \

//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
int ExeCmd(JobsVect* jobs, char* lineSize, char* cmdString, char* LastPath, CmdHistory* hist)
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
		int ChangeDirRes = ERROR_VALUE;
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
				if(ChangeDirRes == ERROR_VALUE){ 		//Error occured
					perror("chdir error");		//TODO The pdf says we should do a specific error.
				} else {						//Directory change succeeded
					cout << LastPath << endl;
					strcpy(LastPath,pwd);
					hist->addString(string("cd -"));
				}
			}

		} else {								//Changing to a new path
			ChangeDirRes = chdir(args[1]);
			if(ChangeDirRes == ERROR_VALUE){ 		//Error occured while switching
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
		if(num_arg != 0) {
			illegal_cmd = true;
		} else {
			//get jobs list
			jobs->printAll();
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "showpid")) 
	{
		if(num_arg != 0) {						//verifying no arguments
			illegal_cmd = true;
		} else {
			int smashPID = getpid();
			cout << "smash pid is " << smashPID << endl;
			hist->addString(string("showpid"));
		}
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
	//FIXME isn't working yet
	else if (!strcmp(cmd, "quit"))
	{
		if (num_arg > 1) { //check 0<=num_arg<=1 (kill)
			illegal_cmd = true;
		} else if (num_arg == 0) {//kill all and quit smash
			exit(0);//no need to release anything
		} else if (strcmp(args[1], "kill")) {//make sure argument is "kill"
			illegal_cmd = true;
		} else { //quit kill ==> send kill request signal to all, 5 secs later force kill
			//all this must be in loop over all jobs
			//get all jobs func TODO amit
			time_t timeFiveSeconds;
			//kill(job.pid, SIGTERM);//FIXME
			timeFiveSeconds = time(NULL);
			cout << " � Sending SIGTERM... ";
			while (difftime(time(NULL), timeFiveSeconds) < 5) {//wait 5 seconds
				//wait 5 seconds
			}
			if (0) {//check if proccess isn't dead 5 seconds later
				//kill(job.pid, SIGKILL);//FIXME
				cout << "(5 sec passed) Sending SIGKILL�" << endl;
			}
			cout << "Done." << endl;
			exit(0);
		}
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
    		case ERROR_VALUE:
				//Error of "fork"
    			perror("Failed to Create Child Process");
    			exit(1); //Only father can run this (and die)
        	case SON_RUNNING :
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
				int i = 1;
				string savedCmd = args[0];
				while(args[i]) {
					savedCmd += (string)(" ") + (string)(args[i]);
					i++;
				}
				if(WEXITSTATUS(status) == 0) {	//Meaning the child was terminated normally
					hist->addString(savedCmd); //TODO History does not show the arguments of the external command. Fix it
				}
				break;
	}
}
//**************************************************************************************
// function name: ExeComp
// Description: executes complicated command
// Parameters: command string
// Returns: 0- if complicated -1- if not
//**************************************************************************************
int ExeComp(char* lineSize, CmdHistory* hist)
{
	char *args[MAX_ARG];
    if ((strstr(lineSize, "|")) || (strstr(lineSize, "<")) || (strstr(lineSize, ">")) || (strstr(lineSize, "*")) || (strstr(lineSize, "?")) || (strstr(lineSize, ">>")) || (strstr(lineSize, "|&")))
    {
    	int pID, status;
        switch(pID = fork()) {
        	case ERROR_VALUE:
    			//Error of "fork"
       			perror("Failed to Create Child Process");
       			exit(1); //Only father can run this (and die)
            case SON_RUNNING :
                // Child Process. Changing the group id.
               	setpgrp();
   			    // Execute an external complicated command through csh.
               	args[0] = "csh";
               	args[1] = "-f";
               	args[2] = "-c";
               	args[3]	= lineSize;
               	args[4] = NULL; //Necessary for csh to work
               	execvp(args[0],args);
  				//If we got here that means execvp failed.
               	perror("Failed to execute external command");
            	exit(1);
    		default:
    			//Father process. Saves the id of the child and wait for it to end
    			int ChildpID = pID;
    			waitpid(ChildpID, &status, WUNTRACED);
    			if(WEXITSTATUS(status) == 0) {	//Meaning the child was terminated normally
     				hist->addString(string(lineSize));
    			}
    			return 0;
    	}
    }
	return -1;
}
//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
int BgCmd(char* lineSize, CmdHistory* hist, JobsVect* jobs)
{

	char* Command;
	const char* delimiters = " \t\n";
	char *args[MAX_ARG];
	if (lineSize[strlen(lineSize)-2] == '&') {//If it is a command to background
		lineSize[strlen(lineSize)-2] = '\0';
		
		//If we get a complicated bg command
		if ((strstr(lineSize, "|")) || (strstr(lineSize, "<")) || (strstr(lineSize, ">")) || (strstr(lineSize, "*")) || (strstr(lineSize, "?")) || (strstr(lineSize, ">>")) || (strstr(lineSize, "|&"))) {
			args[0] = "csh";
			args[1] = "-f";
			args[2] = "-c";
			args[3] = lineSize;
			args[4] = NULL; //Necessary for csh to work
		} else {	//Otherwise we save the command
			args[0] = strtok(lineSize, delimiters);
			if (args[0] == NULL) {	//If we get an empty command we treat it as success
				return 0;
			}
			for (int i = 1; i < MAX_ARG; i++) {	//We saave all the command parameters
				args[i] = strtok(NULL, delimiters);
			}
		}

	}
	//Here we handle the command- put it in list and run it in bg
	int pID;
	switch(pID = fork()) {
		case -1:
	    	//Error of "fork"
	       	perror("Failed to Create Child Process");
	       	exit(1); //Only father can run this (and die)
	    case 0 :
	        // Child Process. Changing the group id.
	        setpgrp();
	        execvp(args[0],args);
	  		//If we got here that means execvp failed.
	        perror("Failed to execute external command");
	        exit(1);
	    default:
	    	//Father process. Doesn't wait for the child to die
	    	string savedCmd;
	    	if(!strcmp(args[0],"csh")) {
	    		savedCmd = (string)lineSize;
	    	} else {
	    		savedCmd = args[0];
	    	}
	    	jobs->insertJob(savedCmd,pID);
	}
	return -1;
}


//		commands.c
//********************************************
#include "commands.h"
#include <unistd.h>
#include <iostream>
#include <string>
#include <cstring>
#include <cctype>
#include "signals.h"

extern JobsVect* jobs;			//Global
extern string globalCmdName;	//Global
extern int globalCmdPID;		//Global

using std::cout;
using std::endl;
using std::string;


//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
int ExeCmd(char* lineSize, char* cmdString, char* LastPath, CmdHistory* hist)
{
	char* cmd;
	char* args[MAX_ARG];
	char pwd[MAX_LINE_SIZE];
	const char* delimiters = " \t\n";
	int i = 0, num_arg = 0;
	bool illegal_cmd = false; // illegal command
    cmd = strtok(lineSize, delimiters);
	if (cmd == NULL) {
		return 0;
	}
   	args[0] = cmd;
	for (i = 1; i < MAX_ARG; i++)
	{
		args[i] = strtok(NULL, delimiters);
		if (args[i] != NULL)
			num_arg++;

	}
/*************************************************/
// Built in Commands
/*************************************************/
	if (!strcmp(cmd, "cd"))
	{
		int ChangeDirRes = ERROR_VALUE;
		if (!getcwd(pwd, MAX_LINE_SIZE)) {
			perror("getcwd error");
			illegal_cmd = true;
		}

		//verifying only 1 argument. An error should be print as illegal command
		if (num_arg != 1 || illegal_cmd == true) {//Added illegal_cmd check in case getcwd failed
			illegal_cmd = true;
		} else if (!strcmp(args[1],"-")) {	//change to last dir
			if (LastPath == NULL) { //No previous path existing
				illegal_cmd = true;
			} else { //Previous path exists
				ChangeDirRes = chdir(LastPath);//Return value
				if (ChangeDirRes == ERROR_VALUE){ //Check success
					//Print is made at end of "cd" section
				} else {		//Directory change succeeded
					cout << LastPath << endl;
					strcpy(LastPath, pwd);
					hist->addString(string("cd -"));
				}
			}

		} else {				//Changing to a new path
			ChangeDirRes = chdir(args[1]);
			if (ChangeDirRes == ERROR_VALUE){  //Error occured while switching
				//Print is made at end of "cd" section
			} else {		//Directory change succeeded
				strcpy(LastPath, pwd);
				string savedCmd = string("cd ");
				savedCmd.append(args[1]);
				hist->addString(savedCmd);
			}
		}
		if (ChangeDirRes == ERROR_VALUE && illegal_cmd == false){ //Check success
			cout << "smash error: > " << args[1] << " - path not found" << endl;
		}
	}

	/*************************************************/
	else if (!strcmp(cmd, "pwd"))
	{
		if(num_arg != 0) {	//verifying no arguments
			illegal_cmd = true;
		} else if(!getcwd(pwd, MAX_LINE_SIZE)) { //Null means getcwd failed
			perror("getcwd error");
		} else {	 //Meaning getcwd succeeded
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
			hist->addString(string("jobs"));
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "showpid"))
	{
		if(num_arg != 0) {			//verifying no arguments
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
		jobs->updateJobs();
		int status;
		Job jobToFg;		//Job to run in fg
		string savedCmd = string("fg");

		if (jobs->isEmpty()) {	//if there are no jobs
			illegal_cmd = true;
		} else {
			if(num_arg == 0) {	//default, which means last job that was inserted
				jobToFg = jobs->getJobById();//Get last job in fg list
			} else if (num_arg == 1) { //A parameter for jobs list given
				if (!(isNum(args[1]))) {//If we are given a char that is not a number
					illegal_cmd = true;
				} else if (atoi(args[1]) <= 0 || atoi(args[1]) > jobs->size()) {//Check legal input
					illegal_cmd = true;
				} else {
					savedCmd += string(" ") + string(args[1]);
					jobToFg = jobs->getJobById(atoi(args[1]));//Return relevant job
				}
			} else { //if we get more than 1 argument
				illegal_cmd = true;
			}
		}
		if (!illegal_cmd) { 					//If command is legal
			globalCmdPID = jobToFg.getPid();	//save command pid for signals
			globalCmdName = jobToFg.getName();	//save command name for signals
			cout << jobToFg.getName() << endl;	//print job name
			//Delete the job before running it in fg to initialize time
			jobs->deleteJob(jobToFg.getPid()); //Remove from job list before running it

			//Handle suspended command, if suspended- wake it up before waiting
			if (jobToFg.isSuspended()) {
				kill(jobToFg.getPid(), SIGCONT);//Wake it up
				cout << "smash > signal SIGCONT was sent to pid " <<
						jobToFg.getPid() << endl;
			}
			waitpid(jobToFg.getPid(), &status, WUNTRACED);
			hist->addString(savedCmd);
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "bg"))
	{
		jobs->updateJobs();
		Job jobToFg;			//Job to run in bg
		string savedCmd = string("bg");
		if (jobs->isEmpty()) {	//if there are no jobs
			illegal_cmd = true;
		} else {
			if(num_arg == 0) {	//default, which means last job that was inserted
				//First find the pid of the last job to be suspended in the vector, than return that job
				jobToFg = jobs->getJobById();//Return relevant job
			} else if (num_arg == 1) {		//A parameter for jobs list given
				if (!(isNum(args[1]))) {	//If we are given a char that is not a number
					illegal_cmd = true;
				} else if (atoi(args[1]) <= 0 || atoi(args[1]) > jobs->size()) {//Check legal input
					illegal_cmd = true;
				} else {
					savedCmd += string(" ") + string(args[1]);
					jobToFg = jobs->getJobById(atoi(args[1]));//Return relevant job
				}
			} else { //if we get more than 1 argument
				illegal_cmd = true;
			}
		}
		if (!illegal_cmd) { //If command is legal
			hist->addString(savedCmd);
			cout << jobToFg.getName() << endl;	//print job name
			if (jobToFg.isSuspended()) { 		//Handle suspended command, if suspended- wake it up
				kill(jobToFg.getPid(), SIGCONT);//Wake it up
				jobs->changeJobRemovalStatus(jobToFg.getPid());//Notify the job isn't suspended anymore
				cout << "smash > signal SIGCONT was sent to pid " << jobToFg.getPid() << endl;
			} else {//Not suspended, so no proccess to wake up. Selected job is already running
				//No need for output or do any action
			}
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
		jobs->updateJobs();//Make sure jobs is updated before using it
   		if (num_arg < 0 || num_arg > 1) {//Validate number of arguments
   			illegal_cmd = true;
   		} else if (num_arg == 0) { //normal quit
   			exit(0);
   		} else if (!(strcmp(args[1], "kill")) == false) { //there's an argument, but it's not kill
   			illegal_cmd = true;
   		} else { //quit kill
   			int jobIndex = 1;//The job id in the vector
   			Job currJob;
   			while(jobs->isEmpty() != true) {//Go over all jobs vector
   				currJob = jobs->getJobById(1);//Get first job in vector for each iteration
				cout << "[" << jobIndex << "] " << currJob.getName() << "- Sending SIGTERM... ";
				kill(currJob.getPid(), SIGTERM);//Try to let it kill itself
				sleep(5);//Give the proc 5 seconds to kill itself
				if(!(waitpid(currJob.getPid(), NULL, WNOHANG))) { //If proccess still exists after 5 seconds
					kill(currJob.getPid(), SIGKILL);//Proc didn't kill itself, force kill it
					cout << "(5 sec passed) Sending SIGKILL... ";
				}
				cout << "Done" << endl;
				jobs->deleteJob(currJob.getPid());//Delete the job killed from the list
				jobIndex++;//Advance the iterator showing the job id in job vector (not pid!)
			}
			exit(0);
   		}
	}
		/*************************************************/
	else if (!strcmp(cmd, "kill"))
	{
		jobs->updateJobs();//Again, probably not needed
		if(num_arg != 2 || strncmp(args[1],"-",1)) {	//If the number of arguments is not correct or "-" isn't leading the second arg
			illegal_cmd = true;
		} else {
			int sigToSend = atoi(strtok(args[1],"-"));
			int idOfJob = atoi(args[2]);
			Job wantedJob = jobs->getJobById(idOfJob);
			string savedCmd = string(args[0]) + string(" ") + string(args[1]) +
									string(" ") + string(args[2]); //To be saved in history

			if(wantedJob.getPid() == NO_PROCESS_RUNNING) { //We got a NULL- meaning this id doesn't exist
				cout << "smash error: > kill " << idOfJob << " - job does not exist"  << endl;
			} else if(sigToSend > 31 || sigToSend < 1) {	//Meaning we got an illegal signal number
				cout << "smash error: > kill " << idOfJob << " - cannot send signal"  << endl;
			} else {
				//Parameters are good if we got here.

				//Handling the case where those are signals we know
				if(sigToSend == SIGINT || sigToSend == SIGTSTP) {
					globalCmdName = wantedJob.getName();
					globalCmdPID = wantedJob.getPid();
					hist->addString(savedCmd);
					jobs->deleteJob(wantedJob.getPid()); //This line is added to prevent "jobs" duplications
					if(sigToSend == SIGINT) {
						handle_CTRL_c(SIGINT);
					} else {
						handle_CTRL_z(SIGTSTP);
					}
				} else {
					if(kill(wantedJob.getPid(), sigToSend) != 0) { //0 means we succeeded
						cout << "smash error: > kill " << idOfJob << " - cannot send signal"  << endl;
					} else {
						cout << "signal " << sigToSend << " was sent to pid " << wantedJob.getPid() << endl;
						hist->addString(savedCmd);
						if(sigToSend == SIGCONT && wantedJob.isSuspended()) {
							jobs->changeSusStatusById(idOfJob, false);
						}
					}
				}
			}
		}
	}
	/*************************************************/
	else // external command
	{
 		ExeExternal(args, cmdString, hist);
	 	return 0;
	}
	//For all the cases where we decided the command is illegal
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
		case CHILD_PROCESS :
			if (setpgrp() == ERROR_VALUE) { //check setpgrp() success
				cout << "setpgrp() failed!" << endl;
			}
			// Execute an external command.
			execvp(args[0], args);
			//If we got here that means execvp failed.
			perror("Failed to execute external command");
			exit(1);
		default:
			globalCmdPID = pID;
			//Father process. Saves the id of the child and wait for it to end
			int ChildpID = pID;
			globalCmdName = cmdString;
			waitpid(ChildpID, &status, WUNTRACED);
			globalCmdPID = NO_PROCESS_RUNNING;
			int i =1;
			string savedCmd = args[0];
			while(args[i]) {
				savedCmd += string(" ") + string(args[i]);
				i++;
			}
			if(WEXITSTATUS(status) == 0) {	//Meaning the child was terminated normally
				hist->addString(savedCmd);
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
int ExeComp(char* cmdString, CmdHistory* hist)
{
    if ((strstr(cmdString, "|")) || (strstr(cmdString, "<")) || (strstr(cmdString, ">")) || (strstr(cmdString, "*")) || (strstr(cmdString, "?")) || (strstr(cmdString, ">>")) || (strstr(cmdString, "|&")))
    {
    	char* args[MAX_ARG];
    	int pID, status;
        switch(pID = fork()) {
        	case ERROR_VALUE:
        		cout << "entered error" << endl;
    			//Error of "fork"
       			perror("Failed to Create Child Process");
       			exit(1); //Only father can run this (and die)
            case CHILD_PROCESS :
                // Child Process. Changing the group id.
				if (setpgrp() == ERROR_VALUE) { //check setpgrp() success
					cout << "setpgrp() failed!" << endl;
				}
   			    // Execute an external complicated command through csh
               	args[0] = (char*)"csh";
            	args[1] = (char*)"-f";
            	args[2] = (char*)"-c";
            	args[3] = cmdString;
               	args[4] = NULL; 		//Necessary for csh to work
               	execvp(args[0], args);
  				//If we got here that means execvp failed.
               	perror("Failed to execute external command");
            	exit(1);
    		default:
    			//Father process. Saves the id of the child and wait for it to end
    			int ChildpID = pID;
				globalCmdPID = pID;
				globalCmdName = cmdString;
				waitpid(ChildpID, &status, WUNTRACED);
				globalCmdPID = NO_PROCESS_RUNNING;
				string savedCmd = string(cmdString);
				savedCmd.erase(savedCmd.end() - 1);
				savedCmd.append("\0"); //Those lines are added because when entering a complex command "Enter" is also pressed.
    			if(WEXITSTATUS(status) == 0) {	//Meaning the child was terminated normally
     				hist->addString(string(savedCmd));
    			}
    			return 0;
    	}
    }
	return ERROR_VALUE;
}
//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
int BgCmd(char* lineSize, CmdHistory* hist, JobsVect* jobs)
{
	jobs->updateJobs();//Make sure jobs is updated before using it
	const char* delimiters = " \t\n";
	char *args[MAX_ARG];
	if (lineSize[strlen(lineSize)-2] == '&') {//If it is a command to background
		lineSize[strlen(lineSize)-2] = '\0';

		//we save the command
		args[0] = strtok(lineSize, delimiters);
		if (args[0] == NULL) {	//If we get an empty command we treat it as success
			return 0;
		}
		for (int i = 1; i < MAX_ARG; i++) {	//We save all the command parameters
			args[i] = strtok(NULL, delimiters);
		}

		//Here we handle the command- put it in list and run it in bg
		int pID;
		switch(pID = fork()) {
			case ERROR_VALUE:
		    	//Error of "fork"
		       	perror("Failed to Create Child Process");
		       	exit(1); //Only father can run this (and die)
		    case CHILD_PROCESS :
		        // Child Process. Changing the group id.
				if (setpgrp() == ERROR_VALUE) { //check setpgrp() success
					cout << "setpgrp() failed!" << endl;
				}
		        execvp(args[0], args);
		  		//If we got here that means execvp failed.
		        perror("Failed to execute external command");
		        exit(1);
		    default:
		    	//Father process. Doesn't wait for the child to die.
		    	string savedCmd = args[0];
		    	for(int i = 1; args[i] != NULL ; i++) {
		    		savedCmd += string(" ") + string(args[i]);
		    	}
		    	jobs->insertJob(savedCmd, pID);
		    	hist->addString(savedCmd);
		    	return 0;
		}
	}
	return ERROR_VALUE;
}


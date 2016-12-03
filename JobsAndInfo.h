/*
 * JobsAndInfo.h
 *
 *  Created on: 22 ����� 2016
 *      Author: Amit
 */

#ifndef JOBSANDINFO_H_
#define JOBSANDINFO_H_

#include <vector>
#include <string>
#include <iostream>
#include <ctime>
#include <sys/wait.h>
#include <cctype>

//Defining macros to be used throughout our program.
#define ERROR_VALUE (-1)
#define NO_PROCESS_RUNNING (-1)
#define JOB_WAS_SUSPENDED (true)
#define HISTORY_MAX (51)
#define CHILD_PROCESS (0)
#define MAX_LINE_SIZE 80
#define KILL_SUCCESS 0

using std::vector;
using std::string;
using std::cout;
using std::endl;

/**
 * CmdHistory class. This class is used to save all the commands that we got
 * (If they have the correct parameters).
 */
class CmdHistory {
private:
	vector <string> _history;
public:
	CmdHistory() {};
	~CmdHistory() {};
	void add(const char* command);
	void addString(string command);
	void printAll();
	int getNumberOfCommands();//used for jobs command
};


/**
 * Job class. This class is used to store commands that goes to the background
 * of our program. It enables different actions on its fields.
 */
class Job {
private:
	string _cmdName;
	int _pid;
	time_t _insertTime;
	bool _isSuspended;
public:

	Job() : _cmdName(),	_pid(NO_PROCESS_RUNNING), _insertTime(0), _isSuspended(false), _isWaitingToBeRemoved(false) {} //default
	Job(string name, int processId, time_t insTime, bool isSuspended, bool isWaitingToBeRemoved = false) : _cmdName(name),
								_pid(processId), _insertTime(insTime), _isSuspended(isSuspended), _isWaitingToBeRemoved(isWaitingToBeRemoved) {}
	bool _isWaitingToBeRemoved;
	int getPid() {
		return _pid;
	}
	string getName() {
			return _cmdName;
	}
	time_t getTime() {
			return (time(NULL) - _insertTime);
	}
	bool isSuspended() {
		return _isSuspended;
	}
	void changeSuspStatus(bool change) {
		this->_isSuspended = change;
	}
};

/**
 * JobsVect class. This class is used to store the jobs that run in background.
 * It is actually a "smart" std::vector that we use for our needs. "Get" methods
 * are implemented here, and the more trickier ones are implemented in .cpp
 */
class JobsVect {
private:
	vector <Job> _allJobs;
public:
	void insertJob(string name, int processId, bool isSuspended = false);
	void deleteJob(int processId);
	void printAll();
	void updateJobs();
	int LastSuspendedPid();
	Job getJobById(int id = -1);
	int getJobIDByPID(int pid);
	void changeJobRemovalStatus(int pid) {//TODO not sure
		(this->_allJobs[getJobIDByPID(pid) - 1])._isWaitingToBeRemoved = true;
	}
	bool isEmpty() {
		return _allJobs.empty();
	}
	int size() {
		return ((this->_allJobs).size());
	}
	void changeSusStatusById(int id, bool state) {
		this->_allJobs[id - 1].changeSuspStatus(state);
	}
};

/**
 * Function used to determine if a string represents a number
 */
bool isNum(const char* str);

#endif /* JOBSANDINFO_H_ */

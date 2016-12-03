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
#include <time.h>
#include <sys/wait.h>
#include <cctype>


#define ERROR_VALUE (-1)
#define NO_PROCESS_RUNNING (-1)
#define JOB_WAS_SUSPENDED (true)
#define HISTORY_MAX (51)
#define CHILD_PROCESS (0)
#define MAX_LINE_SIZE 80
#define KILL_SUCCESS 0

#define NO_JOB_STRING(a) \
	a[0] = '\0'; \

using std::vector;
using std::string;
using std::cout;
using std::endl;

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

class Job {
private:
	string _cmdName;
	int _pid;
	time_t _insertTime;
	bool _isSuspended;
public:
	bool _isWaitingToBeRemoved;
	Job() : _cmdName(),	_pid(NO_PROCESS_RUNNING), _insertTime(0), _isSuspended(false), _isWaitingToBeRemoved(false) {} //default
	Job(string name, int processId, time_t insTime, bool isSuspended, bool isWaitingToBeRemoved = false) : _cmdName(name),
								_pid(processId), _insertTime(insTime), _isSuspended(isSuspended), _isWaitingToBeRemoved(isWaitingToBeRemoved) {}
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
	void reverseSuspensionStatus() {
		this->_isSuspended = ((_isSuspended == true) ? false : true);
	}
};

class JobsVect {
private:
	vector <Job> _allJobs;
public:
	void insertJob(string name, int processId, bool isSuspended = false);
	void deleteJob(int processId);
	void printAll();
	void updateJobs();
	int newestJobPidAndName(string* str);
	int newestJobPid();
	int getPidAndNameByNum(int num, string* str);
	int LastSuspendedPid();
	Job getJobById(int id = -1) {
		if(id == -1) {
			return *(this->_allJobs.end() - 1);
		} else {
			unsigned int unsignedID = id;//To avoid warning of comparison between signed and unsigned TODO amit
			if(id < 1 || unsignedID > (this->_allJobs.size())) {
				return Job();
			}
			//return (this->_allJobs[id]);// TODO notice the mistake we initialy made
			return (this->_allJobs[id - 1]);
		}
	}
	int getJobIDByPID(int pid) {//TODO change to cpp
		vector<Job>::iterator iter = this->_allJobs.begin();
		int id = 1;
		while(iter != this->_allJobs.end()) {
			if(iter->getPid() == pid) {
				break;
			}
			id++;
			iter++;
		}
		return id;
	}
	void changeJobRemovalStatus(int pid) {//TODO not sure
		(this->_allJobs[getJobIDByPID(pid) - 1]).reverseSuspensionStatus();
	}
	
	string LastSuspendedName();
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

bool isNum(const char* str);

#endif /* JOBSANDINFO_H_ */

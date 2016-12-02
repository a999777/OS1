/*
 * JobsAndInfo.h
 *
 *  Created on: 22 бреб„ 2016
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
	Job(string name, int processId, time_t insTime, bool isSuspended) : _cmdName(name),
								_pid(processId), _insertTime(insTime), _isSuspended(isSuspended) {}
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
	string LastSuspendedName();
	bool isEmpty() {
		return _allJobs.empty();
	}
};

bool isNum(const char* str);

#endif /* JOBSANDINFO_H_ */

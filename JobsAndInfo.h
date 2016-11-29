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
//#include "Macros_Defines.h"//TODO

#define ERROR_VALUE (-1)
#define HISTORY_MAX (51)
#define MAX_LINE_SIZE 80

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
	long long int _insertTime;
public:
	Job(string name, int processId, long long int insTime) : _cmdName(name),
								_pid(processId), _insertTime(insTime) {}
	int getPid() {
		return _pid;
	}
	string getName() {
			return _cmdName;
	}
	long long int getTime() {
			return (time(NULL) - _insertTime);
	}
};

class JobsVect {
private:
	vector <Job> _allJobs;
public:
	void insertJob(string name, int processId);
	void deleteJob(int processId);
	void printAll();
};

#endif /* JOBSANDINFO_H_ */

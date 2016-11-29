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
	vector <string> _parameters;
public:
	void add(const char* command, );
	void addString(string command);
	void printAll();
	int getNumberOfCommands();//used for jobs command
};

#endif /* JOBSANDINFO_H_ */

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
};


#endif /* JOBSANDINFO_H_ */

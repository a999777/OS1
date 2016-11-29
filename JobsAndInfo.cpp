/*
 * JobsAndInfo.cpp
 *
 *  Created on: 23 бреб„ 2016
 *      Author: Amit
 */

#include "JobsAndInfo.h"

void CmdHistory::addString(string command) {
	this->_history.push_back(command);
	if(this->_history.size() >= HISTORY_MAX) {
		this->_history.erase(this->_history.begin());
	}
}

void CmdHistory::printAll() {
	vector<string>::iterator it = this->_history.begin();
	while(it != this->_history.end()) {
		cout << *it << endl;
		it++;
	}
}

void JobsVect::insertJob(string name, int processId) {
	Job* job = new Job(name, processId,time(NULL));
	this->_allJobs.push_back(*job);
}

void JobsVect::deleteJob(int processId) {
	vector<Job>::iterator it = this->_allJobs.begin();
	while(it != this->_allJobs.end()) {
		if(it->getPid() == processId) {
			this->_allJobs.erase(it);
			break;
		}
		it++;
	}
}

void JobsVect::printAll() {
	vector<Job>::iterator it = this->_allJobs.begin();
	int counter = 1;
	while(it != this->_allJobs.end()) {
		cout << "[[" << counter << "]] " << it->getName() << " : "
				 << it->getPid() << " " << it->getTime() << endl;
		it++;
		counter++;
	}
}



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
	this->updateJobs();
	Job* job = new Job(name, processId,time(NULL));
	if(this->_allJobs.size() >= 100) {
		cout << "Too many jobs!" << endl;
	} else {
		this->_allJobs.push_back(*job);
	}
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
	this->updateJobs();
	vector<Job>::iterator it = this->_allJobs.begin();
	int counter = 1;
	while(it != this->_allJobs.end()) {
		cout << "[" << counter << "] " << it->getName() << " : "
				 << it->getPid() << " " << it->getTime() << " secs" << endl;
		it++;
		counter++;
	}
}

void JobsVect::updateJobs() {
	int status;
	vector<Job>::iterator it = this->_allJobs.begin();
	while(it != this->_allJobs.end()) {
		if(waitpid(it->getPid(),&status, WNOHANG) != 0 || WEXITSTATUS(status) == 1) {
		//Using this syntax means we get 0 if the process with pid exists
			this->deleteJob(it->getPid());
			break;
		}
		it++;
	}
}


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

int JobsVect::newestJobPidAndName(string* str) {
	this->updateJobs();
	time_t mostRecentTime = this->_allJobs.begin()->getTime();
	int mostRecentPid = this->_allJobs.begin()->getPid();
	string mostRecentName;
	vector<Job>::iterator it = this->_allJobs.begin();
	while(it != this->_allJobs.end()) {
		if(it->getTime() < mostRecentTime ) {
				mostRecentTime = it->getTime();
				mostRecentPid = it->getPid();
				mostRecentName = it->getName();
		}
		it++;
	}
	*str = mostRecentName;
	return mostRecentPid;
}

int JobsVect::getPidAndNameByNum(int num, string* str) {
	*str = this->_allJobs[num].getName();
	return this->_allJobs[num].getPid();
}

bool isNum(const char* str) {
	if(!str) {
		return false;
	}
	for(int i=0; str[i] != NULL ; i++) {
		if(str[i] > 9 || str[i] < 0) {
			return false;
		}
	}
	return true;
}

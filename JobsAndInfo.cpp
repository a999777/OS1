/*
 * JobsAndInfo.cpp
 *
 *  Created on: 23 áðåá× 2016
 *      Author: Amit
 */

#include "JobsAndInfo.h"

/**
 * Adding a command to the history class using its name(and arguments)
 */
void CmdHistory::addString(string command) {
	this->_history.push_back(command);
	if (this->_history.size() >= HISTORY_MAX) {
		this->_history.erase(this->_history.begin());
	}
}

/**
 * A method for printing the entire history so far.
 */
void CmdHistory::printAll() {
	vector<string>::iterator it = this->_history.begin();
	while (it != this->_history.end()) {
		cout << *it << endl;
		it++;
	}
}

/**
 * A method for inserting a job. First it creates the job using the name and
 * the pid, then inserts it.
 */
void JobsVect::insertJob(string name, int processId, bool isSuspended) {
	this->updateJobs();
	Job* job = new Job(name, processId,time(NULL), isSuspended);
	if (this->_allJobs.size() >= 100) {
		cout << "Too many jobs!" << endl;
	} else {
		this->_allJobs.push_back(*job);
	}
}

/**
 * A method for deleting a job from the jobs vector which means deleting a job
 * from the jobs list.
 */
void JobsVect::deleteJob(int processId) {
	vector<Job>::iterator it = this->_allJobs.begin();
	while (it != this->_allJobs.end()) {
		if (it->getPid() == processId) {
			this->_allJobs.erase(it);
			break;
		}
		it++;
	}
}

/**
 * A method for printing the entire jobs vector. That means this is what we
 * need to print when the user hits "jobs".
 */
void JobsVect::printAll() {
	this->updateJobs();
	vector<Job>::iterator it = this->_allJobs.begin();
	int counter = 1;
	while (it != this->_allJobs.end()) {
		cout << "[" << counter << "] " << it->getName() << " : "
				 << it->getPid() << " " << it->getTime() << " secs" ;
		if (it->isSuspended()) {
			cout << " (Stopped)" ;
		}
		cout << endl;
		it++;
		counter++;
	}
}

/**
 * A method necessary in order to keep our jobs list correct. It checks if
 * any process has ended(normally or due to some error) and removes it if so.
 */
void JobsVect::updateJobs() {
	int status;
	vector<Job>::iterator it = this->_allJobs.begin();
	while (it != this->_allJobs.end()) {
		if (waitpid(it->getPid(),&status, WNOHANG) != 0 || WEXITSTATUS(status) == 1) {
		//Using this syntax means we get 0 if the process with pid exists
			this->deleteJob(it->getPid());
			break;
		}
		it++;
	}
}

/**
 * A method for getting the latest suspended process. This is needed in order to
 * enable the "fg" command with no arguments.
 */
int JobsVect::LastSuspendedPid() {
	this->updateJobs();
	int mostRecentPid = -1;
	if (this->_allJobs.begin()->isSuspended()) {
		return this->_allJobs.begin()->getPid();
	}
	vector<Job>::iterator it = this->_allJobs.end() - 1;
	while (it != this->_allJobs.begin()) {
		if (it->isSuspended()) {
			mostRecentPid = it->getPid();
		}
		it--;
	}
	return mostRecentPid;
}

/**
 * A method for getting a job based on its location in the jobs list.
 */
Job JobsVect::getJobById(int id) {
		if(id == -1) {
			return *(this->_allJobs.end() - 1);
		} else {
			unsigned int unsignedID = id;
			if(id < 1 || unsignedID >= (this->_allJobs.size())) {
				return Job();
			}
			return (this->_allJobs[id - 1]);
		}
}

/**
 * A method used for getting the Id of a process- i.e. the location in the list
 * - using its pid. This method is required in order to implement another method.
 */
int JobsVect::getJobIDByPID(int pid) {
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


/**
 * A simple function for checking if a string represents a number.
 */
bool isNum(const char* str) {
	if (!str) {
		return false;
	}
	for (int i = 0; str[i] != '\0' ; i++) {
		if(str[i] > '9' || str[i] < '0') {
			return false;
		}
	}
	return true;
}

/*
 * JobsAndInfo.cpp
 *
 *  Created on: 23 бреб„ 2016
 *      Author: Amit
 */

#include "JobsAndInfo.h"

void CmdHistory::add(char const* command) {
	string newCmd = string(command);
	this->_history.push_back(newCmd);
	if(this->_history.size() >= HISTORY_MAX) {
		this->_history.erase(this->_history.begin());
	}
}

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

int CmdHistory::getNumberOfCommands() {
	return (this->_history.size());
}


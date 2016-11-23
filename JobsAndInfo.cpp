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
	if(this->_history.size() >= 51) {
		this->_history.erase(this->_history.begin());
	}
}

void CmdHistory::addString(string command) {
	this->_history.push_back(command);
	if(this->_history.size() >= 51) {
		this->_history.erase(this->_history.begin());
	}
}

void CmdHistory::printAll() {
	for(int i = 0; i <= this->_history.size() ; i++) {
		cout << this->_history[i] << " command number " << i << endl;
	}
	//TODO something is causing segmentation fault here
}



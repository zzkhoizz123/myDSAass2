/*
 * =========================================================================================
 * Name        : eventLib.cpp
 * Author      : Duc Dung Nguyen
 * Email       : nddung@hcmut.edu.vn
 * Copyright   : Faculty of Computer Science and Engineering - Bach Khoa University
 * Description : library for Assignment 2 - Data structures and Algorithms - Fall 2017
 *               This library contains functions used for event management
 * =========================================================================================
 */

#include "requestLib.h"


void loadRequests(char* fName, L1List<VM_Request> &rList) {
    // TODO: write your code to load requests. Each request is separated by a whitespace

	fstream requestFile;
	requestFile.open(fName, ios::in);
	if (!requestFile) {
		DSAException *ex = new DSAException(-3, "can not open File !");
		throw ex;
	}
	string str;
	while (!requestFile.eof()) {
		getline(requestFile, str);
		if (str.empty())
			continue;
		stringstream ss(str);
		char a[50];
		while (ss >> a) {
			VM_Request *request = new VM_Request();
			char ch;
			double param;
			stringstream ss2(a);
			ss2 >> ch; request->code[0] = ch; request->code[1] = '\0'; // get code
			ss2 >> ch; // get '_'	

			if (request->code[0] == '2' || request->code[0] == '3') {
				ss2 >> param; request->params[0] = param;
				ss2 >> ch >> ch; request->params[1] = (int)ch;
				ss2 >> ch;
				if (ch == ';') {				
					rList.push_back(*request);
					break;
				}
			}
			else {
				int i = 0;
				while (ss2 >> param) {
					request->params[i] = param;
					ss2 >> ch; // get '_' || ';'
					if (ch == ';') {
						break;
					}
					++i;
				}
			}
			rList.push_back(*request);
		}
	}
	requestFile.close();
}

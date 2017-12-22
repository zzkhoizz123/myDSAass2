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
		char a[REQUEST_CODE_SIZE];
		char request[REQUEST_CODE_SIZE];
		while (ss >> a) {
			int i = 0;
			while (a[i] != ';' && a[i] != '\0') {
				request[i] = a[i];
				i++;
			}
			request[i] = '\0';
			VM_Request ne(request);
			rList.push_back(ne);
			strcpy(a, "");
			strcpy(request, "");
		}
	}
	requestFile.close();
}

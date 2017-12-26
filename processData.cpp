/*
 * ==========================================================================================
 * Name        : processData.cpp
 * Description : student code for Assignment 2 - Data structures and Algorithms - Fall 2017
 * ==========================================================================================
 */
#include <iostream>
#include <vector>
#include <functional>
#include <math.h>
#include "requestLib.h"
#include "dbLib.h"

using namespace std;

#define GPS_DISTANCE_ERROR 0.005


bool initVMGlobalData(void** pGData) {
    // TODO: allocate and initialize global data
    // return false if failed

//	L1List<AVLTree<VM_Record>> *dbAVL = new L1List<AVLTree<VM_Record>>;
//	pGData[0] = dbAVL;
	
    return true;
}

void releaseVMGlobalData(void* pGData) {
    // TODO: do your cleanup, left this empty if you don't have any dynamically allocated data

}


bool op1(VM_Record &a, VM_Record &b);
bool op2(VM_Record &a, VM_Record &b);
void ConvertFollowTime(L1List<VM_Record> &recordList, L1List<AVLTree<VM_Record>> &dbAVL);
bool process1Request(VM_Request &request, L1List<AVLTree<VM_Record>> &dbGBAVL);


bool op3(VM_Record &record, double &a, char &locate);
bool process2Request(VM_Request &request, L1List<AVLTree<VM_Record>> &dbAVL);

bool process3Request(VM_Request &request, L1List<AVLTree<VM_Record>> &dbAVL);

bool Helpprocess4Request(AVLNode<VM_Record>* pR, double Along, double Alat, double R, time_t H1, time_t H2);
bool process4Request(VM_Request &request, L1List<AVLTree<VM_Record>> &dbAVL);

int Helpprocess5Request(AVLNode<VM_Record> *pR, double Along, double Alat, double R);
bool process5Request(VM_Request &request, L1List<AVLTree<VM_Record>> &dbAVL);

bool process6Request(VM_Request &request, L1List<AVLTree<VM_Record>> &dbAVL);

//############ global data ################
L1List<AVLTree<VM_Record>> dbGBAVL1; /// using in case arrange with time
int flat1 = 0; /// flat for case arrange with time


bool processRequest(VM_Request &request, L1List<VM_Record> &recordList, void *pGData) {
    // TODO: Your code goes here
    // return false for invlaid events

	if (strcmp(request.code, "1") == 0 ) {
		if (flat1 == 0) {
			ConvertFollowTime(recordList, dbGBAVL1);
			flat1 = 1;
		}
		return process1Request(request, dbGBAVL1);
	}

	if (strcmp(request.code, "2") == 0) {
		if (flat1 == 0) {
			ConvertFollowTime(recordList, dbGBAVL1);
			flat1 = 1;
		}
		return process2Request(request, dbGBAVL1);
	}

	if (strcmp(request.code, "3") == 0) {
		if (flat1 == 0) {
			ConvertFollowTime(recordList, dbGBAVL1);
			flat1 = 1;
		}
		return process3Request(request, dbGBAVL1);
	}

	if (strcmp(request.code, "4") == 0) {
		if (flat1 == 0) {
			ConvertFollowTime(recordList, dbGBAVL1);
			flat1 = 1;
		}
		return process4Request(request, dbGBAVL1);
	}

	if (strcmp(request.code, "5") == 0) {
		if (flat1 == 0) {
			ConvertFollowTime(recordList, dbGBAVL1);
			flat1 = 1;
		}
		return process5Request(request, dbGBAVL1);
	}

    return true;
}


bool op1(VM_Record &a, VM_Record &b) { /// this function compare follow to time
	if (a.timestamp < b.timestamp) return true;
	return false;
}

bool op2(VM_Record &a, VM_Record &b) {
	if (a.timestamp > b.timestamp) return true;
	return false;
}

void ConvertFollowTime(L1List<VM_Record> &recordList, L1List<AVLTree<VM_Record>> &dbAVL) {
	L1Item<VM_Record> *pL = recordList.getHead();
	
	while (pL) {
		L1Item<AVLTree<VM_Record>> *pA = dbAVL.getHead();
		int check = 0;
		while (pA != NULL && check != 1) {
			if (strcmp(pL->data.id, pA->data.getpRoot()->data.id) == 0) { /// conincide with id
				pA->data.Insert(pL->data, op1);
				check = 1;
			}
			pA = pA->pNext;
		} 
		if (check == 0) { /// have not had this id yet => insert
			AVLTree<VM_Record> *ret = new AVLTree<VM_Record>;
			ret->Insert(pL->data, op1);
			dbAVL.insertHead(*ret);
		}
		pL = pL->pNext;
	}
}

//############################# process1Request #################################
bool process1Request(VM_Request &request, L1List<AVLTree<VM_Record>> &dbGBAVL) {

	string strtime;	
	stringstream ss1, ss2, ss3;
	ss1 << (int)request.params[0]; string ret1 = ss1.str();
	if (ret1.length() == 1) ret1 = "000" + ret1;
	else if (ret1.length() == 2) ret1 = "00" + ret1;
	else if (ret1.length() == 3) ret1 = "0" + ret1;
	const char *id1 = ret1.c_str();

	ss2 << (int)request.params[1]; string ret2 = ss2.str();
	if (ret2.length() == 1) ret2 = "000" + ret2;
	else if (ret2.length() == 2) ret2 = "00" + ret2;
	else if (ret2.length() == 3) ret2 = "0" + ret2;
	const char *id2 = ret2.c_str();

	ss3 << (int)request.params[2]; strtime = ss3.str();
	if (strtime.length() == 1) strtime = "00000" + strtime;
	else if (strtime.length() == 2) strtime = "0000" + strtime;
	else if (strtime.length() == 3) strtime = "000" + strtime;
	else if (strtime.length() == 4) strtime = "00" + strtime;
	else if (strtime.length() == 5) strtime = "0" + strtime;
	tm tm_date = { 0 };	

	//------------------ lay ngay thang nam trong database
	char Date[26];
	strPrintTime(Date, dbGBAVL1.getHead()->data.getpRoot()->data.timestamp);
	string str = Date;

	tm_date.tm_mon = stoi(str.substr(5, 2)) - 1;
	tm_date.tm_mday = stoi(str.substr(8,2));
	tm_date.tm_year = stoi(str.substr(0,4)) - 1900;
	tm_date.tm_hour = stoi(strtime.substr(0,2), nullptr, 10) ;
	tm_date.tm_min = stoi(strtime.substr(2, 2), nullptr, 10);
	tm_date.tm_sec = stoi(strtime.substr(4, 2), nullptr, 10);

	time_t timecompare = mktime(&tm_date);

	L1Item<AVLTree<VM_Record>> *pRun = dbGBAVL.getHead();
	VM_Record record1; int check1 = 0; /// kiem tra xem co id1 trong db ko
	VM_Record record2; int check2 = 0; /// kiem tra xem co id2 trong db ko

	while (pRun) {
		if (strcmp(id1, pRun->data.getpRoot()->data.id) == 0) {
			check1 = 1; /// co id1 trong db
			VM_Record recordTemp; recordTemp.timestamp = timecompare;
			record1 = pRun->data.Find(recordTemp, op1, op2);
			if (strcmp(record1.id, "0") == 0 || strcmp(record1.id, "") == 0) {
				cout << request.code << ": " << "-1\n"; /// record1 khong co tai thoi diem do	
				return true;
			}
		}

		if (strcmp(id2, pRun->data.getpRoot()->data.id) == 0) {
			check2 = 1; /// co id2 trong db
			VM_Record recordTemp; recordTemp.timestamp = timecompare;
			record2 = pRun->data.Find(recordTemp, op1, op2);
			if (strcmp(record2.id, "0") == 0 || strcmp(record2.id, "") == 0) {
				cout << request.code << ": " << "-1\n";/// record2 khong co tai thoi diem do
				return true;
			}
		}
		pRun = pRun->pNext;
	}

	if (check1 == 0 || check2 == 0) return false;
	char longLocation, laLocation;
	if (record1.longitude >= record2.longitude) longLocation = 'E';
	else longLocation = 'W';
	if (record2.latitude >= record2.latitude) laLocation = 'N';
	else laLocation = 'S';

	cout << request.code << ": " << longLocation << " " << laLocation << " " << distanceEarth(
		record1.latitude, record1.longitude, record2.latitude, record2.longitude) << '\n';
	return true;
}

//################################## process2Request ######################################
bool op3(VM_Record &record, double &a, char &locate) {
	switch (locate) {
	case 'E': {
		if (record.longitude >= a) return true;
		return false;
	}
	case 'W': {
		if (record.longitude < a) return true;
		return false;
	}
	case 'N': {
		if (record.latitude >= a) return true;
		return false;
	}
	case 'S': {
		if (record.latitude < a)return true;
		return false;
	}
	default:
		return false;
	}
}

bool process2Request(VM_Request &request, L1List<AVLTree<VM_Record>> &dbAVL) {

	double locateCompare = request.params[0];
	char locate = (int)request.params[1];

	if (locate != 'E' && locate != 'W') {
		return false;
	}

	L1Item<AVLTree<VM_Record>> *pRun = dbAVL.getHead();
	int number = 0;

	while (pRun) {
		bool check = pRun->data.Traverse_NLR(op3, locateCompare, locate);
		if (check == true) {
			++number;
		}
		pRun = pRun->pNext;
	}	
	cout << request.code << ": " << number << '\n';
	return true;
}

//############################ process3Request ###############################
bool process3Request(VM_Request &request, L1List<AVLTree<VM_Record>> &dbAVL) {

	double locateCompare = request.params[0];
	char locate = (int)request.params[1];

	if (locate != 'N' && locate != 'S') {
		return false;
	}

	L1Item<AVLTree<VM_Record>> *pRun = dbAVL.getHead();
	int number = 0;

	while (pRun) {
		bool check = pRun->data.Traverse_NLR(op3, locateCompare, locate);
		if (check == true) {
			++number;
		}
		pRun = pRun->pNext;
	}
	cout << request.code << ": " << number << '\n';
	return true;
}

//################################### process4Request ####################################
bool Helpprocess4Request(AVLNode<VM_Record>* pR, double Along, double Alat, double R, time_t H1, time_t H2) {
	
	if (pR) {
		if (pR->data.timestamp < H1) return Helpprocess4Request(pR->pRight, Along, Alat, R, H1, H2);
		else if (pR->data.timestamp > H2) return Helpprocess4Request(pR->pLeft, Along, Alat, R, H1, H2);
		else {
			if (distanceEarth(Alat, Along, pR->data.latitude, pR->data.longitude) <= R)	return true;		
			else return (Helpprocess4Request(pR->pLeft, Along, Alat, R, H1, H2) || Helpprocess4Request(pR->pRight, Along, Alat, R, H1, H2));
		}
	}
	else return false;
}

bool process4Request(VM_Request &request, L1List<AVLTree<VM_Record>> &dbAVL) {

	double Along, Alat, R;
	time_t H1, H2;

	/// get them from request
	Along = request.params[0];
	Alat = request.params[1];
	R = request.params[2];

	tm tm_date1 = { 0 };
	tm tm_date2 = { 0 };
	//------------------ lay ngay thang nam trong database
	char Date[26];
	strPrintTime(Date, dbGBAVL1.getHead()->data.getpRoot()->data.timestamp);
	string str = Date;

	tm_date2.tm_mon = tm_date1.tm_mon = stoi(str.substr(5, 2)) - 1;
	tm_date2.tm_mday = tm_date1.tm_mday = stoi(str.substr(8, 2));
	tm_date2.tm_year =  tm_date1.tm_year = stoi(str.substr(0, 4)) - 1900;
	tm_date1.tm_hour = (int)request.params[3]; tm_date2.tm_hour = (int)request.params[4];
	H1 = mktime(&tm_date1);
	H2 = mktime(&tm_date2);
	
	//------------------- thuc thi request
	L1Item<AVLTree<VM_Record>> *pRun = dbAVL.getHead();
	int count = 0;

	while (pRun) {
		bool check = Helpprocess4Request(pRun->data.getpRoot(), Along, Alat, R, H1, H2);
		if (check == true) ++count;				
		pRun = pRun->pNext;
	}

	cout << request.code << ": " << count << '\n';
	return true;
}

//#################################### process5Request ######################################
int Helpprocess5Request(AVLNode<VM_Record> *pR, double Along, double Alat, double R) {
	static int count = 0;
	if (pR == NULL) return count;
	else {
		if (distanceEarth(pR->data.latitude, pR->data.longitude, Alat, Along) <= R) ++count;
		int a = Helpprocess5Request(pR->pLeft, Along, Alat, R);
		int b = Helpprocess5Request(pR->pRight, Along, Alat, R);
		return (a > b ? a : b);
	}
}

bool process5Request(VM_Request &request, L1List<AVLTree<VM_Record>> &dbAVL) {

	stringstream ss1;
	ss1 << (int)request.params[0]; string ret = ss1.str();
	if (ret.length() == 1) ret = "000" + ret;
	else if (ret.length() == 2) ret = "00" + ret;
	else if (ret.length() == 3) ret = "0" + ret;
	const char *id = ret.c_str();

	double Along, Alat, R;
	Along = request.params[1];
	Alat = request.params[2];
	R = request.params[3];

	int count = -1;
	L1Item<AVLTree<VM_Record>> *pRun = dbAVL.getHead();
	while (pRun) {
		if (strcmp(id, pRun->data.getpRoot()->data.id) == 0) {
			count = Helpprocess5Request(pRun->data.getpRoot(), Along, Alat, R);
			break;
		}
		pRun = pRun->pNext;
	}
	if (count == -1) return false;
	cout << request.code << ": " << count << '\n';
	return true;
}

//############################### process6Request ##################################

string solveTime(string datetime) { // 12/05/2016 00:41:05


}



bool process6Request(VM_Request &request, L1List<AVLTree<VM_Record>> &dbAVL) {




	return true;
}
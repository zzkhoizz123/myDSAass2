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

	return true;
}
void releaseVMGlobalData(void* pGData) {
	// TODO: do your cleanup, left this empty if you don't have any dynamically allocated data
}

// Khai bao nguyen mau ham
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

bool process7Request(VM_Request &request, L1List<AVLTree<VM_Record>> &dbAVL);

bool process8Request(VM_Request &request, L1List<AVLTree<VM_Record>> &dbAVL, L1List<AVLTree<VM_Record>> &dbAVL2);

bool process9Request(VM_Request &request, L1List<AVLTree<VM_Record>> &dbAVL, L1List<AVLTree<VM_Record>> &dbAVL2);

//############ global data ################
L1List<AVLTree<VM_Record>> dbGBAVL1; /// using in case arrange with time
int flat1 = 0; /// flat for case arrange with time

L1List<AVLTree<VM_Record>> dbGBAVL2; /// using in order to resolve 


bool processRequest(VM_Request &request, L1List<VM_Record> &recordList, void *pGData) {

	if (request.code[0] == '1') {
		if (flat1 == 0) {
			ConvertFollowTime(recordList, dbGBAVL1);
			flat1 = 1;
		}
		return process1Request(request, dbGBAVL1);
	}

	if (request.code[0] == '2') {
		if (flat1 == 0) {
			ConvertFollowTime(recordList, dbGBAVL1);
			flat1 = 1;
		}
		return process2Request(request, dbGBAVL1);
	}

	if (request.code[0] == '3') {
		if (flat1 == 0) {
			ConvertFollowTime(recordList, dbGBAVL1);
			flat1 = 1;
		}
		return process3Request(request, dbGBAVL1);
	}

	if (request.code[0] == '4') {
		if (flat1 == 0) {
			ConvertFollowTime(recordList, dbGBAVL1);
			flat1 = 1;
		}
		return process4Request(request, dbGBAVL1);
	}

	if (request.code[0] == '5') {
		if (flat1 == 0) {
			ConvertFollowTime(recordList, dbGBAVL1);
			flat1 = 1;
		}
		return process5Request(request, dbGBAVL1);
	}

	if (request.code[0] == '6') {
		if (flat1 == 0) {
			ConvertFollowTime(recordList, dbGBAVL1);
			flat1 = 1;
		}
		return process6Request(request, dbGBAVL1);
	}

	if (request.code[0] == '7') {
		if (flat1 == 0) {
			ConvertFollowTime(recordList, dbGBAVL1);
			flat1 = 1;
		}
		return process7Request(request, dbGBAVL1);
	}

	if (request.code[0] == '8') {
		if (flat1 == 0) {
			ConvertFollowTime(recordList, dbGBAVL1);
			flat1 = 1;
		}
		return process8Request(request, dbGBAVL1, dbGBAVL2);
	}

	if (request.code[0] == '9') {
		if (flat1 == 0) {
			ConvertFollowTime(recordList, dbGBAVL1);
			flat1 = 1;
		}
		return process9Request(request, dbGBAVL1, dbGBAVL2);
	}

	return false;
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
	// 1_0001_0003_001128

	string ret1, ret2;
	string req = request.code;

	req = req.substr(2);
	int post = req.find('_');
	ret1 = req.substr(0, post);

	req = req.substr(post + 1);
	post = req.find('_');
	ret2 = req.substr(0, post);

	string strtime = req.substr(post + 1);

	if (ret1.length() == 1) ret1 = "000" + ret1;
	else if (ret1.length() == 2) ret1 = "00" + ret1;
	else if (ret1.length() == 3) ret1 = "0" + ret1;
	const char *id1 = ret1.c_str();

	if (ret2.length() == 1) ret2 = "000" + ret2;
	else if (ret2.length() == 2) ret2 = "00" + ret2;
	else if (ret2.length() == 3) ret2 = "0" + ret2;
	const char *id2 = ret2.c_str();

	tm tm_date = { 0 };

	//------------------ lay ngay thang nam trong database
	char Date[26];
	strPrintTime(Date, dbGBAVL1.getHead()->data.getpRoot()->data.timestamp);
	string str = Date;

	tm_date.tm_mon = stoi(str.substr(5, 2)) - 1;
	tm_date.tm_mday = stoi(str.substr(8, 2));
	tm_date.tm_year = stoi(str.substr(0, 4)) - 1900;
	tm_date.tm_hour = stoi(strtime.substr(0, 2), nullptr, 10);
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
	if (record1.latitude >= record2.latitude) laLocation = 'N';
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
	// 2_-122.678_E

	string req = request.code;
	req = req.substr(2);

	int post = req.find('_');
	string strLocate = req.substr(0, post);

	req = req.substr(post + 1);

	double locateCompare = stod(strLocate);
	char locate = req[0];

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

	string req = request.code;
	req = req.substr(2);

	int post = req.find('_');
	string strLocate = req.substr(0, post);

	req = req.substr(post + 1);

	double locateCompare = stod(strLocate);
	char locate = req[0];

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
	//4_-122.41204_37.743_5.02_00_01

	string strLong, strLat, strR, strH1, strH2;

	string req = request.code;
	req = req.substr(2);

	int post = req.find('_');
	strLong = req.substr(0, post);

	req = req.substr(post + 1);
	post = req.find('_');
	strLat = req.substr(0, post);

	req = req.substr(post + 1);
	post = req.find('_');
	strR = req.substr(0, post);

	req = req.substr(post + 1);
	post = req.find('_');
	strH1 = req.substr(0, post);

	req = req.substr(post + 1);
	strH2 = req.substr(0);

	double Along, Alat, R;
	int intH1, intH2;
	time_t H1, H2;

	/// get them from request
	Along = stod(strLong);
	Alat = stod(strLat);
	R = stod(strR);
	intH1 = stoi(strH1);
	intH2 = stoi(strH2);

	tm tm_date1 = { 0 };
	tm tm_date2 = { 0 };
	//------------------ lay ngay thang nam trong database
	char Date[26];
	strPrintTime(Date, dbGBAVL1.getHead()->data.getpRoot()->data.timestamp);
	string str = Date;

	tm_date2.tm_mon = tm_date1.tm_mon = stoi(str.substr(5, 2)) - 1;
	tm_date2.tm_mday = tm_date1.tm_mday = stoi(str.substr(8, 2));
	tm_date2.tm_year = tm_date1.tm_year = stoi(str.substr(0, 4)) - 1900;
	tm_date1.tm_hour = intH1; tm_date2.tm_hour = intH2;
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
	static bool check = false;
	if (pR == NULL) return count;
	else {
		if (distanceEarth(pR->data.latitude, pR->data.longitude, Alat, Along) <= R) {
			if (check == false) {
				count++;
				check = true;
			}
		}
		else check = false;
		int a = Helpprocess5Request(pR->pLeft, Along, Alat, R);
		int b = Helpprocess5Request(pR->pRight, Along, Alat, R);
		return (a > b ? a : b);
	}
}

bool process5Request(VM_Request &request, L1List<AVLTree<VM_Record>> &dbAVL) {
	//5_0002_-122.41204_37.743_2.01

	string req = request.code;
	string ret, strLong, strLat, strR;
	req = req.substr(2);

	int post = req.find('_');
	ret = req.substr(0, post);

	req = req.substr(post + 1);
	post = req.find('_');
	strLong = req.substr(0, post);

	req = req.substr(post + 1);
	post = req.find('_');
	strLat = req.substr(0, post);

	req = req.substr(post + 1);
	strR = req.substr(0);

	if (ret.length() == 1) ret = "000" + ret;
	else if (ret.length() == 2) ret = "00" + ret;
	else if (ret.length() == 3) ret = "0" + ret;
	const char *id = ret.c_str();

	double Along, Alat, R;
	Along = stod(strLong);
	Alat = stod(strLat);
	R = stod(strR);

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
struct MapOver {
	char	id[ID_MAX_LENGTH];
	int		valid; // record is valid when it is in H1 to H2
	int		check_2km;
	int		check_500m;
	int		check_300m;

	MapOver() :valid(0), check_2km(0), check_500m(0), check_300m(0) {
		id[0] = 0;
	}
	MapOver(MapOver& a) :valid(a.valid), check_2km(a.check_2km), check_500m(a.check_500m), check_300m(a.check_300m) {
		strcpy(id, a.id);
	}
};


void traverseCheckOver(AVLNode<VM_Record> *pR, MapOver &Map, double Along, double Alat, time_t H1, time_t H2) {
	if (pR) {
		if (pR->data.timestamp < H1) traverseCheckOver(pR->pRight, Map, Along, Alat, H1, H2);
		else if (pR->data.timestamp > H2) traverseCheckOver(pR->pLeft, Map, Along, Alat, H1, H2);
		else {
			Map.valid = 1;
			double ret = distanceEarth(Alat, Along, pR->data.latitude, pR->data.longitude);
			if (ret <= 2.0) Map.check_2km = 1;
			if (ret <= 0.5) Map.check_500m = 1;
			if (ret <= 0.3) Map.check_300m = 1;
			traverseCheckOver(pR->pLeft, Map, Along, Alat, H1, H2);
			traverseCheckOver(pR->pRight, Map, Along, Alat, H1, H2);
		}
	}
}

time_t solveTime_Over(time_t datetime) { // 2016/12/05 00:41
	char Date[26];
	strPrintTime(Date, datetime);
	string str = Date;
	int hour, min, day, month, year;

	year = stoi(str.substr(0, 4));
	month = stoi(str.substr(5, 2));
	day = stoi(str.substr(8, 2));
	hour = stoi(str.substr(11, 2));
	min = stoi(str.substr(14, 2));

	if (min >= 15) min = min - 15;
	else { // min < 15
		if (hour > 0) {
			hour = hour - 1;
			min = 60 - (15 - min);
		}
		else { // hour = 0
			day = day - 1;
			hour = 23;
			min = 60 - (15 - min);
		}
	}
	tm tm_date = { 0 };
	tm_date.tm_mon = month - 1;
	tm_date.tm_mday = day;
	tm_date.tm_year = year - 1900;
	tm_date.tm_hour = hour;
	tm_date.tm_min = min;

	return mktime(&tm_date);
}

bool op_MapOver(MapOver &a, MapOver &b) {
	if (strcmp(a.id, b.id) > 0) return true;
	return false;
}

void Print_Over(MapOver *a, int N) {
	for (int i = 0; i < N; i++) {
		cout << " " << a[i].id;
	}
}

bool process6Request(VM_Request &request, L1List<AVLTree<VM_Record>> &dbAVL) {

	//6_-122.41204_37.7473_1_0042

	string req = request.code;
	string strLong, strLat, strM, strtime;
	req = req.substr(2);

	int post = req.find('_');
	strLong = req.substr(0, post);

	req = req.substr(post + 1);
	post = req.find('_');
	strLat = req.substr(0, post);

	req = req.substr(post + 1);
	post = req.find('_');
	strM = req.substr(0, post);

	req = req.substr(post + 1);
	strtime = req.substr(0);

	double Along = stod(strLong);
	double Alat = stod(strLat);
	int M = stod(strM);

	//------------------ lay ngay thang nam trong database
	tm tm_date = { 0 };
	char Date[26];
	strPrintTime(Date, dbGBAVL1.getHead()->data.getpRoot()->data.timestamp); // 2016-12-05 00:41:04
	string str = Date;

	tm_date.tm_mon = stoi(str.substr(5, 2)) - 1;
	tm_date.tm_mday = stoi(str.substr(8, 2));
	tm_date.tm_year = stoi(str.substr(0, 4)) - 1900;
	tm_date.tm_hour = stoi(strtime.substr(0, 2));
	tm_date.tm_min = stoi(strtime.substr(2, 2));
	tm_date.tm_sec = 59;

	time_t H2 = mktime(&tm_date);
	time_t H1 = solveTime_Over(H2);

	L1Item<AVLTree<VM_Record>> *pRun = dbAVL.getHead();
	MapOver *mapArr_500m = new MapOver[dbAVL.getSize()]; int run_500m = 0;
	MapOver *mapArr_500mExc = new MapOver[dbAVL.getSize()]; int run_500mExc = 0;
	MapOver *mapArr_Total = new MapOver[dbAVL.getSize()]; int run_Total = 0;
	int count_2km = 0, count_300m = 0;
	while (pRun) {
		MapOver *map = new MapOver();
		strcpy(map->id, pRun->data.getpRoot()->data.id);
		traverseCheckOver(pRun->data.getpRoot(), *map, Along, Alat, H1, H2);
		if (map->check_2km == 1) ++count_2km;
		if (map->check_500m == 1) {
			mapArr_500m[run_500m++] = *map;
		}
		else if (map->valid == 1) mapArr_500mExc[run_500mExc++] = *map;
		if (map->check_300m == 1) ++count_300m;
		if (map->valid == 1) mapArr_Total[run_Total++] = *map;
		pRun = pRun->pNext;
	}

	if (count_2km < M) {
		shellSort(mapArr_Total, run_Total, op_MapOver);
		cout << request.code << ":";
		Print_Over(mapArr_Total, run_Total); cout << " - \n";
	}
	else if (count_300m > 0.75*M) {
		shellSort(mapArr_Total, run_Total, op_MapOver);
		cout << request.code << ": -";
		Print_Over(mapArr_Total, run_Total); cout << '\n';
	}
	else {
		shellSort(mapArr_500m, run_500m, op_MapOver);
		shellSort(mapArr_500mExc, run_500mExc, op_MapOver);
		cout << request.code << ":"; Print_Over(mapArr_500m, run_500m); cout << " -"; Print_Over(
			mapArr_500mExc, run_500mExc); cout << '\n';
	}
	return true;
}

//############################## process7Request ###############################
struct MapCongestion {
	char	id[ID_MAX_LENGTH];
	int		valid; /// valid when record is in H1 to H2
	int		check_500m;
	int		check_2kmTo1km;
	double	distance_2kmTo1km;

	MapCongestion() : valid(0), check_500m(0), check_2kmTo1km(0), distance_2kmTo1km(0.0) {
		id[0] = 0;
	}
	MapCongestion(MapCongestion& a) : valid(a.valid), check_500m(a.check_500m), check_2kmTo1km(a.check_2kmTo1km),
		distance_2kmTo1km(a.distance_2kmTo1km) {
		strcpy(id, a.id);
	}
};

double traverseCheckCongestion(AVLNode<VM_Record> *pR, MapCongestion &Map, double Along, double Alat, time_t H1, time_t H2) {
	static double max_2kmTo1km = 0.0;
	if (pR == NULL) return max_2kmTo1km;
	else {
		if (pR->data.timestamp < H1) return traverseCheckCongestion(pR->pRight, Map, Along, Alat, H1, H2);
		else if (pR->data.timestamp > H2) return traverseCheckCongestion(pR->pLeft, Map, Along, Alat, H1, H2);
		else {
			double ret = distanceEarth(Alat, Along, pR->data.latitude, pR->data.longitude);
			Map.valid = 1;
			if (ret <= 0.5) Map.check_500m = 1;
			if (ret <= 2.0 && ret >= 1.0) {
				Map.check_2kmTo1km = 1;
				max_2kmTo1km = ret > max_2kmTo1km ? ret : max_2kmTo1km;
			}
			double a = traverseCheckCongestion(pR->pLeft, Map, Along, Alat, H1, H2);
			double b = traverseCheckCongestion(pR->pRight, Map, Along, Alat, H1, H2);
			return (a > b ? a : b);
		}
	}
}

bool op_MapCong_id(MapCongestion &a, MapCongestion &b) {
	if (strcmp(a.id, b.id) > 0) return true;
	return false;
}

bool op_MapCong_dis(MapCongestion &a, MapCongestion &b) {
	if (a.distance_2kmTo1km < b.distance_2kmTo1km) return true;
	return false;
}

time_t solveTime_Cog(time_t datetime) { /// 2016/12/05 00:41
	char Date[26];
	strPrintTime(Date, datetime);
	string str = Date;
	int hour, min, day, month, year;

	year = stoi(str.substr(0, 4));
	month = stoi(str.substr(5, 2));
	day = stoi(str.substr(8, 2));
	hour = stoi(str.substr(11, 2));
	min = stoi(str.substr(14, 2));

	if (min < 30) min = min + 30;
	else { // min >= 30
		if (hour == 23) {
			day = day + 1;
			hour = 0;
			min = 60 - min;
		}
		else { // hour != 23
			hour = hour + 1;
			min = 60 - min;
		}
	}
	tm tm_date = { 0 };
	tm_date.tm_mon = month - 1;
	tm_date.tm_mday = day;
	tm_date.tm_year = year - 1900;
	tm_date.tm_hour = hour;
	tm_date.tm_min = min;

	return mktime(&tm_date);
}

void Print_Congestion(MapCongestion *a, int N) {
	for (int i = 0; i < N; i++) {
		cout << " " << a[i].id;
	}
}

bool process7Request(VM_Request &request, L1List<AVLTree<VM_Record>> &dbAVL) {
	//7_-122.40224_37.743_100_3.02_0020;

	string req = request.code;
	string strLong, strLat, strM, strR, strtime;
	req = req.substr(2);

	int post = req.find('_');
	strLong = req.substr(0, post);

	req = req.substr(post + 1);
	post = req.find('_');
	strLat = req.substr(0, post);

	req = req.substr(post + 1);
	post = req.find('_');
	strM = req.substr(0, post);

	req = req.substr(post + 1);
	post = req.find('_');
	strR = req.substr(0, post);

	req = req.substr(post + 1);
	strtime = req.substr(0);

	double Along = stod(strLong);
	double Alat = stod(strLat);
	int M = stod(strM);

	//------------------ lay ngay thang nam trong database
	tm tm_date = { 0 };
	char Date[26];
	strPrintTime(Date, dbAVL.getHead()->data.getpRoot()->data.timestamp); /// 2016-12-05 00:41:04
	string str = Date;

	tm_date.tm_mon = stoi(str.substr(5, 2)) - 1;
	tm_date.tm_mday = stoi(str.substr(8, 2));
	tm_date.tm_year = stoi(str.substr(0, 4)) - 1900;
	tm_date.tm_hour = stoi(strtime.substr(0, 2));
	tm_date.tm_min = stoi(strtime.substr(2, 2));

	time_t H1 = mktime(&tm_date);
	time_t H2 = solveTime_Cog(H1);

	L1Item<AVLTree<VM_Record>> *pRun = dbAVL.getHead();

	MapCongestion *map_Total = new MapCongestion[dbAVL.getSize()]; int run_Total = 0;
	MapCongestion *map_2kmTo1km = new MapCongestion[dbAVL.getSize()]; int run_2kmTo1km = 0;
	MapCongestion *map_2kmTo1kmExc = new MapCongestion[dbAVL.getSize()]; int run_2kmTo1kmExc = 0;
	int count_500m = 0;
	while (pRun) {
		MapCongestion *map = new MapCongestion();
		strcpy(map->id, pRun->data.getpRoot()->data.id);
		double dis = traverseCheckCongestion(pRun->data.getpRoot(), *map, Along, Alat, H1, H2);

		if (map->check_500m == 1) ++count_500m;
		if (map->check_2kmTo1km == 1) {
			map->distance_2kmTo1km = dis;
			map_2kmTo1km[run_2kmTo1km++] = *map;
		}
		else if (map->valid == 1) map_2kmTo1kmExc[run_2kmTo1kmExc++] = *map;
		if (map->valid == 1) map_Total[run_Total++] = *map;
		pRun = pRun->pNext;
	}

	if (count_500m < 0.7*M) {
		shellSort(map_Total, run_Total, op_MapCong_id);
		cout << request.code << ":  -"; Print_Congestion(map_Total, run_Total); cout << '\n';
	}
	else {
		shellSort(map_2kmTo1km, run_2kmTo1km, op_MapCong_dis);
		int runRet = run_2kmTo1km;
		run_2kmTo1km = (int)(run_2kmTo1km * 0.75); /// cac xe khong mac ket
		runRet = runRet - run_2kmTo1km; /// cac xe mac ket
		for (int i = run_2kmTo1kmExc; i < run_2kmTo1kmExc + runRet; i++) {
			map_2kmTo1kmExc[i] = map_2kmTo1km[i - run_2kmTo1kmExc + run_2kmTo1km];
		}
		run_2kmTo1kmExc += runRet;
		shellSort(map_2kmTo1km, run_2kmTo1km, op_MapCong_id);
		shellSort(map_2kmTo1kmExc, run_2kmTo1kmExc, op_MapCong_id);

		cout << request.code << ":"; Print_Congestion(map_2kmTo1kmExc, run_2kmTo1kmExc);
		cout << " -"; Print_Congestion(map_2kmTo1km, run_2kmTo1km); cout << '\n';
	}
	return true;
}

//############################# process8Request ##############################

time_t solveTime_Mal(time_t datetime) { /// 2016/12/05 00:41
	char Date[26];
	strPrintTime(Date, datetime);
	string str = Date;
	int hour, min, day, month, year;

	year = stoi(str.substr(0, 4));
	month = stoi(str.substr(5, 2));
	day = stoi(str.substr(8, 2));
	hour = stoi(str.substr(11, 2));
	min = stoi(str.substr(14, 2));

	tm tm_date = { 0 };
	tm_date.tm_mon = month - 1;
	tm_date.tm_mday = day;
	tm_date.tm_year = year - 1900;
	tm_date.tm_hour = hour;
	tm_date.tm_min = min;
	tm_date.tm_sec = 59;
	return mktime(&tm_date);
}


bool Helpprocess8Request(AVLNode<VM_Record> *pR, double Along, double Alat, double R, time_t H1, time_t H2) {
	/// do not have malfunction

	if (pR == NULL) return false;
	else {
		if (pR->data.timestamp < H1) return Helpprocess8Request(pR->pRight, Along, Alat, R, H1, H2);
		else if (pR->data.timestamp > H2) return Helpprocess8Request(pR->pLeft, Along, Alat, R, H1, H2);
		else {
			if (distanceEarth(Alat, Along, pR->data.latitude, pR->data.longitude) <= R) return true;
			else {
				return (Helpprocess8Request(pR->pLeft, Along, Alat, R, H1, H2) || Helpprocess8Request(
					pR->pRight, Along, Alat, R, H1, H2));
			}
		}
	}
}

bool process8Request(VM_Request &request, L1List<AVLTree<VM_Record>> &dbAVL, L1List<AVLTree<VM_Record>> &dbAVL2) {
	// 8_-122.354_37.739_5.02_0034

	string strLong, strLat, strR, strtime;
	string req = request.code;
	req = req.substr(2);

	int post = req.find('_');
	strLong = req.substr(0, post);

	req = req.substr(post + 1);
	post = req.find('_');
	strLat = req.substr(0, post);

	req = req.substr(post + 1);
	post = req.find('_');
	strR = req.substr(0, post);

	req = req.substr(post + 1);
	strtime = req.substr(0);

	double Along = stod(strLong);
	double Alat = stod(strLat);
	double R = stod(strR);

	//------------------ lay ngay thang nam trong database
	tm tm_date = { 0 };
	char Date[26];
	strPrintTime(Date, dbAVL.getHead()->data.getpRoot()->data.timestamp); /// 2016-12-05 00:41:04
	string str = Date;

	tm_date.tm_mon = stoi(str.substr(5, 2)) - 1;
	tm_date.tm_mday = stoi(str.substr(8, 2));
	tm_date.tm_year = stoi(str.substr(0, 4)) - 1900;
	tm_date.tm_hour = stoi(strtime.substr(0, 2));
	tm_date.tm_min = stoi(strtime.substr(2, 2));

	time_t H1 = mktime(&tm_date);
	time_t H2 = solveTime_Mal(H1);

	L1Item<AVLTree<VM_Record>> *pRun = dbAVL.getHead();	
	L1Item<AVLTree<VM_Record>> *pRev = dbAVL.getHead();

	int i = 0;
	while (pRun) {
		if (Helpprocess8Request(pRun->data.getpRoot(), Along, Alat, R, H1, H2) == true) {
			if (i == 0) {
				dbAVL2.insertHead(pRun->data);
				dbAVL.setHead(pRun->pNext);				
				pRun = pRun->pNext;
				dbAVL.downSize();
				i = 0;
				continue;
			}
			else {
				dbAVL2.insertHead(pRun->data);
				pRev->pNext = pRun->pNext;
				pRun = pRun->pNext;
				dbAVL.downSize();
				i = 1;
			}
		}
		else {
			pRev = pRun;
			pRun = pRun->pNext;
			i = 1;
		}
	}

	cout << request.code << ":";
	L1Item<AVLTree<VM_Record>> * p = dbAVL2.getHead();
	while (p) {
		cout << " " << p->data.getpRoot()->data.id;
		p = p->pNext;
	}
	cout << "\n";
	return true;
}

//####################################### process9request #####################################
bool process9Request(VM_Request &request, L1List<AVLTree<VM_Record>> &dbAVL, L1List<AVLTree<VM_Record>> &dbAVL2) {


	return true;
}
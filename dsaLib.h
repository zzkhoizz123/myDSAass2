/*
 * =========================================================================================
 * Name        : dsaLib.h
 * Author      : Duc Dung Nguyen
 * Email       : nddung@hcmut.edu.vn
 * Copyright   : Faculty of Computer Science and Engineering - Bach Khoa University
 * Description : library for Assignment 2 - Data structures and Algorithms - Fall 2017
 * =========================================================================================
 */

#ifndef A02_DSALIB_H
#define A02_DSALIB_H

#include <string>
#include <math.h>
#include <vector>
//#include"dbLib.h"

using namespace std;

class DSAException {
    int     _error;
    string  _text;
public:

    DSAException() : _error(0), _text("Success") {}
    DSAException(int err) : _error(err), _text("Unknown Error") {}
    DSAException(int err, const char* text) : _error(err), _text(text) {}

    int getError() { return _error; }
    string& getErrorText() { return _text; }
};

template <class T>
struct L1Item {
    T data;
    L1Item<T> *pNext;
    L1Item() : pNext(NULL) {}
    L1Item(T &a) : data(a), pNext(NULL) {}
};


template <class T>
class L1List {
	L1Item<T>   *_pHead;// The head pointer of linked list
	size_t      _size;// number of elements in this list
public:
	L1List() : _pHead(NULL), _size(0) {}
	~L1List() {
		this->clean();
	}

	void    clean();
	bool    isEmpty() {
		return _pHead == NULL;
	}
	size_t  getSize() {
		return _size;
	}

	L1Item<T>* getHead() {
		return this->_pHead;
	}

	void setHead(L1Item<T> *p) {
		this->_pHead = p;
	}

	void downSize() {
		this->_size--;
	}

	T&      at(int i) {}
	T&      operator[](int i);

	bool    find(T& a, int& idx) {}
	int     insert(int i, T& a) {}
	int     remove(int i) {}

	int     push_back(T& a);
	int     insertHead(T& a);

	int     removeHead();
	int     removeLast();
	
	void    traverse(void(*op)(T&)) {
		L1Item<T>   *p = _pHead;
		while (p) {
			op(p->data);
			p = p->pNext;
		}
	}
	void    traverse(void(*op)(T&, void*), void* pParam) {
		L1Item<T>   *p = _pHead;
		while (p) {
			op(p->data, pParam);
			p = p->pNext;
		}
	}
};


template<class T>
T& L1List<T>:: operator[](int i) {
	if (this->isEmpty()) {
		DSAException *ex = new DSAException(-1, "database is empty");
		throw ex;
	}
	L1Item<T>* pNew = this->_pHead;
	while (i > 0 && pNew != NULL) {
		pNew = pNew->pNext;
		i--;
	}
	if (pNew) return pNew->data;

	DSAException *ex = new DSAException(-2, "excessive limitation");
	throw ex;
}


template<class T>
void L1List<T>::clean() {
	while (!isEmpty()) {
		removeHead();
	}
}

/// Insert item to the end of the list
/// Return 0 if success
template <class T>
int L1List<T>::push_back(T &a) {
    if (_pHead == NULL) {
        _pHead = new L1Item<T>(a);
    }
    else {
        L1Item<T>   *p = _pHead;
        while (p->pNext) p = p->pNext;
        p->pNext = new L1Item<T>(a);
    }
    _size++;
    return 0;
}

/// Insert item to the front of the list
/// Return 0 if success
template <class T>
int L1List<T>::insertHead(T &a) {
    L1Item<T>   *p = new L1Item<T>(a);
    p->pNext = _pHead;
    _pHead = p;
    _size++;
    return 0;
}

/// Remove the first item of the list
/// Return 0 if success
template <class T>
int L1List<T>::removeHead() {
    if(_pHead) {
        L1Item<T>* p = _pHead;
        _pHead = p->pNext;
        delete p;
        _size--;
        return 0;
    }
    return -1;
}

/// Remove the last item of the list
/// Return 0 if success
template <class T>
int L1List<T>::removeLast() {
    if(_pHead) {
        if(_pHead->pNext) {
            L1Item<T>* prev = _pHead;
            L1Item<T>* pcur = prev->pNext;
            while(pcur->pNext) {
                prev = pcur;
                pcur = pcur->pNext;
            }
            delete pcur;
            prev->pNext = NULL;
        }
        else {
            delete _pHead;
            _pHead = NULL;
        }
        _size--;
        return 0;
    }
    return -1;
}

/************************************************************************
 * This section is for AVL tree
 ************************************************************************/
//template <class T>
//struct AVLNode {
//    T           _data;
//    AVLNode<T>   *_pLeft, *_pRight;
//#ifdef AVL_USE_HEIGHT
//	int         _bFactor;
//	AVLNode(T &a) : _data(a), _pLeft(NULL), _pRight(NULL), _bFactor(0) {}
//#else
//	int         _height;
//	AVLNode(T &a) : _data(a), _pLeft(NULL), _pRight(NULL), _height(1) {}
//#endif
//};

enum BFactor { EH = 0, LH = -1, RH = 1 };
template<class T>
struct AVLNode {
	T data;
	BFactor b;
	AVLNode<T> *pLeft, *pRight;
	AVLNode() : b(EH), pLeft(NULL), pRight(NULL) {}
	AVLNode(T &a) :data(a), b(EH), pLeft(NULL), pRight(NULL) {}
	AVLNode(T&a, AVLNode<T>*pL, AVLNode<T>*pR) :data(a), b(EH), pLeft(pL), pRight(pR) {}
};

template<class T>
class AVLTree {
private:
	AVLNode<T> *pRoot;
protected:
	void RotRight(AVLNode<T> *&pR);
	void RotLeft(AVLNode<T> *&pR);
	void RotLR(AVLNode<T> *&pR);
	void RotRL(AVLNode<T> *&pR);
	bool BalanceLeft(AVLNode<T> *&pR);
	bool BalanceRight(AVLNode<T> *&pR);
	void Destroy(AVLNode<T> *pR);
	bool Traverse_NLR(AVLNode<T>* pR, bool(*op)(T&, double &, char &), double &a, char &locate);
	bool Insert(T &a, AVLNode<T>*&pR);
	bool Insert(T &a, AVLNode<T>*&pR, bool(*op)(T &, T&));
	bool Remove(T &a, AVLNode<T>*&pR);
	int Height(AVLNode<T>* pR);
public:

	AVLTree() :pRoot(NULL) {}
	~AVLTree() { Destroy(this->pRoot); }

	AVLNode<T>* getpRoot() {
		return this->pRoot;
	}

	bool Traverse_NLR( bool(*op)(T&, double &, char &), double &a, char &locate) {
		if (this->pRoot) { return Traverse_NLR(this->pRoot, op, a, locate); }
		return true;
	}

	int Height() {
		return this->Height(this->pRoot);
	}

	bool Insert(T &a) {
		return Insert(a, this->pRoot);
	}


	void Insert(T& a, bool(*op)(T &, T &)) {
		Insert(a, this->pRoot, op);
	}

	bool Remove(T &a) {
		return Remove(a, this->pRoot);
	}

};

template<class T>
bool AVLTree<T>::Remove(T &a, AVLNode<T>*&pR) {
	if (pR == NULL) return false;
	if (a < pR->data) {
		if (Remove(a, pR->pLeft) == false) return false;
		if (pR->b == LH) { pR->b = EH; return true; }
		if (pR->b == EH) { pR->b = RH; return false; }
		return !BalanceRight(pR);
	}
	if (a > pR->data) {
		if (Remove(a, pR->pRight) == false) return false;
		if (pR->b == RH) { pR->b = EH; return true; }
		if (pR->b == EH) { pR->b = LH; return false; }
		return !BalanceLeft(pR);
	}
	// if a == pR->data
	if (pR->pLeft == NULL && pR->pRight == NULL) {
		delete pR;
		pR = NULL;
		return true;
	}
	if (pR->pLeft == NULL) {
		AVLNode<T> *p = pR;
		pR = pR->pRight;
		delete p;
		return true;
	}

	if (pR->pRight == NULL) {
		AVLNode<T> *p = pR;
		pR = pR->pLeft;
		delete p;
		return true;
	}

	// if pR->pLeft != NULL && pR->pRight != NULL
	AVLNode<T> *p = pR->pRight;
	while (p->pLeft) p = p->pLeft;
	pR->data = p->data; // clone data
	if (Remove(pR->data, pR->pRight)) { // khong giam chieu cao
										// thuc hien xoa node root
		if (pR->pLeft == NULL && pR->pRight == NULL) {
			delete pR;
			pR = NULL;
			return true;
		}
		if (pR->pLeft == NULL) {
			AVLNode<T> *p = pR;
			pR = pR->pRight;
			delete p;
			return true;
		}
		if (pR->pRight == NULL) {
			AVLNode<T> *p = pR;
			pR = pR->pLeft;
			delete p;
			return true;
		}
	}
}

template<class T>
bool AVLTree<T>::BalanceLeft(AVLNode<T> *&pR) {
	if (pR->b == EH) { pR->b = LH; return true; } // o day da xu ly luon ca truong hop pR->pLeft->b == EH 
	if (pR->b == RH) { pR->b = EH; return false; }
	// if pR->b == LH
	if (pR->pLeft->b == LH) { //L-L
		RotRight(pR);
		pR->b = pR->pRight->b = EH;
		return false;
	}
	// else if pR->pLeft->b == RH
	// Left - Right
	RotLR(pR);
	if (pR->b == LH) {
		pR->b = pR->pLeft->b = EH;
		pR->pRight->b = RH;
		return false;
	}
	// else if pR->b == RH
	pR->b = pR->pRight->b = EH;
	pR->pLeft->b = LH;
	return false;
}


template<class T>
bool AVLTree<T>::BalanceRight(AVLNode<T> *&pR) {
	if (pR->b == EH) { pR->b = RH; return true; }
	if (pR->b == LH) { pR->b = EH; return false; }
	if (pR->pRight->b == RH) { // R-R
		RotLeft(pR);
		pR->b = pR->pLeft->b = EH;
		return false;
	}
	// Right-Left
	RotRL(pR);
	if (pR->b == LH) {
		pR->b = pR->pRight->b = EH;
		pR->pLeft->b = LH;
		return false;
	}
	// pR->b == LH
	pR->b = pR->pLeft->b = EH;
	pR->pRight->b = RH;
	return false;
}

template<class T>
int AVLTree<T>::Height(AVLNode<T>* pR) {
	if (pR == NULL) return 0;
	else {
		int lDepth = Height(pR->pLeft);
		int rDepth = Height(pR->pRight);
		if (lDepth > rDepth)
			return lDepth + 1;
		else return rDepth + 1;
	}
}

template<class T>
void AVLTree<T>::Destroy(AVLNode<T>*pR) {
	if (pR == NULL) return;
	Destroy(pR->pLeft);
	Destroy(pR->pRight);
	delete pR;
}

template<class T>
bool AVLTree<T>::Traverse_NLR(AVLNode<T>* pR, bool(*op)(T&, double &, char &), double &a, char &locate) {
	if (pR) {
		if (op(pR->data, a, locate) == false) return false;
		return (Traverse_NLR(pR->pLeft, op, a, locate) && Traverse_NLR(pR->pRight, op, a, locate));
	}
	else { // pR == NULL
		return true;
	}
}

template<class T>
void AVLTree<T>::RotRight(AVLNode<T> *&pR) {
	if (pR == NULL) return;
	if (pR->pLeft == NULL) return;
	AVLNode<T> *p = pR->pLeft;
	pR->pLeft = p->pRight;
	p->pRight = pR;
	pR = p;
}

template<class T>
void AVLTree<T>::RotLeft(AVLNode<T> *&pR) {
	if (pR == NULL) return;
	if (pR->pRight == NULL) return;
	AVLNode<T> *p = pR->pRight;
	pR->pRight = p->pLeft;
	p->pLeft = pR;
	pR = p;
}

template<class T>
void AVLTree<T>::RotLR(AVLNode<T> *&pR) {
	RotLeft(pR->pLeft);
	RotRight(pR);
}

template<class T>
void AVLTree<T>::RotRL(AVLNode<T> *&pR) {
	RotRight(pR->pRight);
	RotLeft(pR);
}

template<class T>
bool AVLTree<T>::Insert(T &a, AVLNode<T>*&pR) {
	if (pR == NULL) { pR = new AVLNode<T>(a); return true; }
	if (a < pR->data) { // insert left
		if (Insert(a, pR->pLeft) == false) return false;
		return this->BalanceLeft(pR);
	} // end of insert
	else {
		if (Insert(a, pR->pRight) == false) return false;
		return this->BalanceRight(pR);
	}
}

template<class T>
bool AVLTree<T>::Insert(T &a, AVLNode<T>*&pR, bool(*op)(T &, T&)) { // this boolean function compare smaller
	if (pR == NULL) { pR = new AVLNode<T>(a); return true; }
	if (op(a, pR->data) == true) { // insert left
		if (Insert(a, pR->pLeft, op) == false) return false;
		return this->BalanceLeft(pR);
	} // end of insert
	else {
		if (Insert(a, pR->pRight, op) == false) return false;
		return this->BalanceRight(pR);
	}
}


// ################## Sort ##################
template<class T>
void shellSort(T *pD, int N, bool (*op)(T& , T&)) {
	if (N == 0) return;
	for (int gap = N / 2; gap > 0; gap = gap / 2) {
		for (int i = gap; i < N; i++) {
			T temp = pD[i];
			int j;
			for (j = i; j >= gap && (op(pD[j - gap], temp) == true); j = j - gap) {
				pD[j] = pD[j - gap];
			}
			pD[j] = temp;
		}
	}
}

#endif //A02_DSALIB_H

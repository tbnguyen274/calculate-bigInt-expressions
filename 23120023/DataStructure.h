#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

#include <stdio.h>
#include <iostream>
using namespace std;

template <class T>
struct Node {
	T data;
	Node<T>* next;
};

template <class T>
struct Stack {
	Node<T>* top;
};


/*
Functions for Node
*/
template <class T>
Node<T>* createNode(T x) {
	Node<T>* p = new Node<T>;
	p->data = x;
	p->next = NULL;
	return p;
}

/*
Functions for Stack
*/
template <class T>
void initStack(Stack<T>& s) {
	s.top = NULL;
}

template <class T>
void push(Stack<T>& s, T x) {
	Node<T>* p = new Node<T>;
	p->data = x;
	p->next = s.top;
	s.top = p;
}

template <class T>
T pop(Stack<T>& s) {
	if (s.top == NULL) {
		return T();
	}
	Node<T>* p = s.top;
	s.top = s.top->next;
	T x = p->data;
	delete p;
	return x;
}

template <class T>
bool isEmpty(const Stack<T>& s) {
	return s.top == NULL;
}

template <class T>
T getTop(const Stack<T>& s) {
	if (s.top == NULL) {
		return T();
	}
	return s.top->data;
}

template <class T>
void print(const Stack<T>& s) {
	Node<T>* p = s.top;
	while (p != NULL) {
		cout << p->data << " ";
		p = p->next;
	}
	cout << endl;
}

template <class T>
void clear(Stack<T>& s) {
	while (s.top != NULL) {
		Node<T>* p = s.top;
		s.top = s.top->next;
		delete p;
	}
}

#endif // DATASTRUCTURE_H
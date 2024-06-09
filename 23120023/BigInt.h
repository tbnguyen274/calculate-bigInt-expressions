#ifndef BIGINT_H
#define BIGINT_H

#include "DataStructure.h"
#include <string.h>
using namespace std;

// Define a BigInt struct which counts up to thousands of digits
struct BigInt {
	Node<int>* head;
	Node<int>* tail;
	bool sign;  // true for POSITIVE, false for NEGATIVE
};

void init(BigInt& a);

void addHead(BigInt& a, int x);

void addTail(BigInt& a, int x);

BigInt create(const char* str);

void print(const BigInt& a);

char* toString(const BigInt& a);

char* toString(int x);

int countDigits(const BigInt& a);

int countDigits(const int& x);

bool operator<(const BigInt& a, const BigInt& b);

bool operator==(const BigInt& a, const BigInt& b);

BigInt operator-(const BigInt& a);

BigInt operator++(BigInt& a, int notused);

BigInt abs(const BigInt& a);

BigInt operator+(const BigInt& a, const BigInt& b);

BigInt operator-(const BigInt& a, const BigInt& b);

BigInt operator*(const BigInt& a, const BigInt& b);

BigInt operator/(const BigInt& a, const BigInt& b);

void deleteBigInt(BigInt& a);

#endif // BIGINT_H
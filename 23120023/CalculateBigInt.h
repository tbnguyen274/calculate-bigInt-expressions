#ifndef CALCULATEBIGINT_H
#define CALCULATEBIGINT_H
#include "BigInt.h"

int precedence(char op);

bool isNum(char c);

bool isOperator(char c);

void infixToPostfix(const char* infix, char* postfix);

BigInt calculatePostfix(const char* postfix);

int getMaxLenOfLines(const char* fileName);

void run();
#endif // CALCULATEBIGINT_H
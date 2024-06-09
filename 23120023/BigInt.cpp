#define _CRT_SECURE_NO_WARNINGS
#include "BigInt.h"

void init(BigInt& a) {
	a.head = a.tail = NULL;
	a.sign = true;	// Positive
}


void addHead(BigInt& a, int x) {
	Node<int>* p = createNode(x);
	if (a.head == NULL) {
		a.head = a.tail = p;
	}
	else {
		p->next = a.head;
		a.head = p;
	}
}


void addTail(BigInt& a, int x) {
	Node<int>* p = createNode(x);
	if (a.head == NULL) {
		a.head = a.tail = p;
	}
	else {
		a.tail->next = p;
		a.tail = p;
	}
}


BigInt create(const char* str) {
	BigInt a;
	init(a);
	Node<int>* p = NULL;
	int i = 0;
	if (str[0] == '-') {
		a.sign = false;
		i = 1;
	}
	else {
		a.sign = true;
	}
	for (; str[i] != '\0'; i++) {
		int x = str[i] - '0';
		addTail(a, x);
	}
	return a;
}


void print(const BigInt& a) {
	// Print the sign
	if (!a.sign) {
		cout << "-";
	}
	if (a.head == NULL) {
		cout << "0";
	}
	else {
		Node<int>* p = a.head;
		while (p != NULL) {
			cout << p->data;
			p = p->next;
		}
	}
	cout << endl;
}


char* toString(const BigInt& a) {
	int length = countDigits(a);
	if (length <= 0) {
		return NULL;
	}

	if (!a.sign) {
		++length;
	}
	char* str = new char[length + 1];
	
	int i = 0;
	if (!a.sign) {
		str[i++] = '-';
	}
	
	Node<int>* p = a.head;
	while (p != NULL) {
		str[i++] = p->data + '0';
		p = p->next;
	}
	str[i] = '\0';
	return str;
}

int countDigits(const BigInt& a) {
	if (a.head == NULL) {
		return 1;
	}
	Node<int>* p = a.head;
	int count = 0;
	while (p != NULL) {
		count++;
		p = p->next;
	}
	return count;
}


char* toString(int x) {
	int length = countDigits(x);
	if (x < 0) {
		length++;
	}
	char* str = new char[length + 1];
	sprintf(str, "%d", x);
	return str;
}


int countDigits(const int& x) {
	if (x == 0) {
		return 1;
	}
	int count = 0;
	int y = x;
	while (y > 0) {
		count++;
		y /= 10;
	}
	return count;
}


BigInt operator-(const BigInt& a) {
	BigInt result = a;
	result.sign = !result.sign;
	return result;
}


BigInt operator++(BigInt& a, int notused) {
	BigInt result = a;
	a = a + create("1");
	return result;
}


BigInt abs(const BigInt& a) {
	BigInt b = a;
	b.sign = true;
	return b;
}


BigInt operator+(const BigInt& a, const BigInt& b) {
	// case 1: a(+) + b(-)
	if (a.sign && !b.sign) {
		return a - abs(b);
	}

	// case 2: a(-) + b(+)
	else if (!a.sign && b.sign) {
		return b - abs(a);
	}

	// case 3: a(-) + b(-)
	else if (!a.sign && !b.sign) {
		return -(abs(a) + abs(b));
	}
	
	//case 4: a(+) + b(+)
	Stack<int> s1, s2;
	initStack(s1);
	initStack(s2);
	Node<int>* p = a.head;
	Node<int>* q = b.head;

	// Push all digits into the two stacks
	while (p != NULL) {
		push(s1, p->data);
		p = p->next;
	}
	while (q != NULL) {
		push(s2, q->data);
		q = q->next;
	}

	// Perform the addition
	BigInt c;
	init(c);
	int carry = 0;
	while (!isEmpty(s1) || !isEmpty(s2)) {
		int sum = carry;
		if (!isEmpty(s1)) {
			sum += pop(s1);
		}
		if (!isEmpty(s2)) {
			sum += pop(s2);
		}
		carry = sum / 10;
		sum %= 10;
		addHead(c, sum);
	}
	if (carry > 0) {
		addHead(c, carry);
	}

	clear(s1);
	clear(s2);

	return c;
}


bool operator<(const BigInt& a, const BigInt& b) {
	if (a.sign && !b.sign) {
		return false;
	}
	else if (!a.sign && b.sign) {
		return true;
	}
	else if (!a.sign && !b.sign) {
		return abs(b) < abs(a);
	}

	Node<int>* p = a.head;
	Node<int>* q = b.head;

	// Compare the number of digits
	int a_length = 0;
	int b_length = 0;
	while (p != NULL) {
		a_length++;
		p = p->next;
	}
	while (q != NULL) {
		b_length++;
		q = q->next;
	}
	if (a_length < b_length) {
		return true;
	}
	else if (a_length > b_length) {
		return false;
	}

	// If a and b have the same number of digits, compare digit by digit
	p = a.head;
	q = b.head;
	
	while (p != NULL && q != NULL) {
		if (p->data < q->data) {
			return true;
		}
		else if (p->data > q->data) {
			return false;
		}
		p = p->next;
		q = q->next;
	}
	if (p == NULL && q != NULL) {
		return true;
	}
	return false;
}


BigInt operator-(const BigInt& a, const BigInt& b) {
	// case 1: a = b
	if (a == b) {
		return create("0");
	}

	// case 2: a(+) - b(+) && a > b
	if (a.sign && b.sign && b < a) {
		Stack<int> s1, s2;
		initStack(s1);
		initStack(s2);
		Node<int>* p = a.head;
		Node<int>* q = b.head;

		// Push all digits into the two stacks
		while (p != NULL) {
			push(s1, p->data);
			p = p->next;
		}
		while (q != NULL) {
			push(s2, q->data);
			q = q->next;
		}

		// Perform the subtraction
		BigInt c;
		init(c);
		int borrow = 0;
		while (!isEmpty(s1) || !isEmpty(s2)) {
			int diff = borrow;
			if (!isEmpty(s1)) {
				diff += pop(s1);
			}
			if (!isEmpty(s2)) {
				diff -= pop(s2);
			}
			if (diff < 0) {
				diff += 10;
				borrow = -1;
			}
			else {
				borrow = 0;
			}
			addHead(c, diff);
		}

		// Check the sign
		if (borrow < 0) {
			c.sign = false;
		}
		else {
			c.sign = true;
		}

		// Remove leading temps
		while (c.head != NULL 
			&& c.head->data == 0 
			&& c.head != c.tail) 
		{
			Node<int>* temp = c.head;
			c.head = c.head->next;
			delete temp;
		}

		clear(s1);
		clear(s2);

		return c;
	}

	// case 3: a(+) - b(+) && a < b
	else if (a.sign && b.sign && a < b) {
		BigInt c = b - a;
		c.sign = false;
		return c;
	}

	// case 4: a(+) - b(-)
	else if (a.sign && !b.sign) {
		BigInt c = a + abs(b);
		c.sign = true;
		return c;
	}

	//case 5: a(-) - b(+)
	else if (!a.sign && b.sign) {
		BigInt c = abs(a) + b;
		c.sign = false;
		return c;
	}

	// case 6: a(-) - b(-)
	else {
		BigInt c = abs(b) - abs(a);
		return c;
	}
}


BigInt operator*(const BigInt& a, const BigInt& b) {
	BigInt result = create("0");

	if (a.head == NULL || b.head == NULL) {
		return result;
	}
	// If a or b = 0, return 0
	if (a == create("0") || b == create("0")) {
		return result;
	}

	Stack<int> s;
	initStack(s);

	Node<int>* q = b.head;
	while (q != NULL) {
		push(s, q->data);
		q = q->next;
	}

	BigInt ten = create("10");
	int pos = 0;

	while (!isEmpty(s)) {
		BigInt temp = create("0");
		int b_digit = pop(s);
		// Multiply a by b_digit
		for (int i = 0; i < b_digit; i++) {
			temp = temp + abs(a);
		}
		if (pos == 0) {
			result = result + temp;
		}
		else {
			for (int i = 0; i < pos; i++) {
				BigInt temp2 = temp;
				// Multiply temp by 10
				for (int j = 1; j < 10; j++) {
					temp = temp + temp2;
				}
				deleteBigInt(temp2);
			}
			result = result + temp;
		}
		pos++;
		deleteBigInt(temp);
	}

	// Check the sign
	if (a.sign != b.sign) {
		result.sign = false;
	}
	else {
		result.sign = true;
	}

	deleteBigInt(ten);
	clear(s);

	return result;
}


BigInt operator/(const BigInt& a, const BigInt& b) {
	BigInt result = create("0");

	// Check for division by zero
	if (b.head == NULL || b == create("0")) {
		cout << "Division by zero\n";
		return result;
	}

	if (a == create("0")) {
		return result;
	}

	// If |a| < |b|, return 0
	if (abs(a) < abs(b)) {
		return result;
	}

	// Perform the division
	BigInt dividend = abs(a);
	BigInt divisor = abs(b);
	BigInt ten = create("10");

	// Find the number of digits in the divisor
	int divisor_length = countDigits(divisor);

	// Find the first part of the dividend to divide
	BigInt first;
	init(first);
	Node<int>* p = dividend.head;

	for (int i = 0; i < divisor_length; i++) {
		addTail(first, p->data);
		if (i < divisor_length - 1) {
			p = p->next;
		}
	}

	// If first < divisor, add the next digit to first
	if (first < divisor) {
		p = p->next;
		addTail(first, p->data);
	}
	
	BigInt quotient = create("0");
	Node<int>* q = quotient.head;

	while (p != NULL) {
		// Find the first quotient
		for (int i = 9; i >= 0; i--) {
			char* str = toString(i);
			BigInt temp = create(str);
			BigInt product = divisor * temp;
			if (product < first || product == first) {
				quotient = temp;
				break;
			}
			delete[] str;
		}

		// Find the remainder
		BigInt remainder = first - (quotient * divisor);

		// Append the quotient to the result
		result = result * ten;
		result = result + quotient;

		// Move to the next digit
		p = p->next;
		if (p == NULL) {
			break;
		}

		// Update the first
		first = remainder * ten;
		char* str = toString(p->data);
		first = first + create(str);
		delete[] str;
	}
	
	// Check the sign
	if (a.sign != b.sign) {
		result.sign = false;
	}
	else {
		result.sign = true;
	}

	return result;
}


bool operator==(const BigInt& a, const BigInt& b) {
	if (a.sign != b.sign) {
		return false;
	}

	Node<int>* p = a.head;
	Node<int>* q = b.head;
	while (p != NULL && q != NULL) {
		if (p->data != q->data) {
			return false;
		}
		p = p->next;
		q = q->next;
	}
	if (p == NULL && q == NULL) {
		return true;
	}
	return false;
}


void deleteBigInt(BigInt& a) {
	Node<int>* p = a.head;
	while (p != NULL) {
		Node<int>* temp = p;
		p = p->next;
		delete temp;
	}
}
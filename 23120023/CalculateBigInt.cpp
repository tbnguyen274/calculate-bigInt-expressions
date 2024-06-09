#define _CRT_SECURE_NO_WARNINGS
#include "CalculateBigInt.h"

int precedence(char op) {
    if (op == '+' || op == '-') {
        return 1;
    }
    if (op == '*' || op == ':' || op == '/') {
        return 2;
    }
    return 0;
}


bool isNum(char c) {
    return c >= '0' && c <= '9';
}


bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == ':' || c == '/';
}


void infixToPostfix(const char* infix, char* postfix) {
    Stack<char> stack;  // Stack to store operators and parentheses
    Stack<bool> isNegativeStack;    // Stack to store negative numbers
    Stack<bool> isNegativeParenthesis;  // Stack to store negative numbers/expressions in parentheses

    initStack(stack);
    initStack(isNegativeStack);
    initStack(isNegativeParenthesis);

    int i = 0, j = 0;
    bool isNegativeInParenthesis = false;   // Check if there's a negative number in parentheses such as (-5)

    while (infix[i] != '\0') {
        char c = infix[i];

        // Update the status of negative numbers in parentheses
        if (isNegativeInParenthesis && (c == ')' || c == ' ')) {
            isNegativeInParenthesis = true;
        }
        else if (isNegativeInParenthesis && !isNum(c)) {
			isNegativeInParenthesis = false;
		}        
        
        // Case 1: c is an operator
        if (isOperator(c)) {
            // Check if c is a negative sign
            if (c == '-' && (i == 0 || infix[i - 1] == '(' || isOperator(infix[i - 2]) || isOperator(infix[i - 1]))) {
                if (infix[i + 1] == '(' || infix[i + 2] == '(') {
					push(isNegativeParenthesis, true);
				}
                else {
                    push(isNegativeStack, true);
                }
                if (infix[i - 1] == '(') {
                    isNegativeInParenthesis = true;
                }
                
                postfix[j++] = '0';
                postfix[j++] = ' ';
                if ((isOperator(infix[i - 2]) && infix[i + 1] == ' ') ||
                    ((i == 0 || infix[i - 1] == '(') && infix[i + 1] == ' ')) {
                    i = i + 1;  // skip the next space if there's a space in a negative number
                }
                i++;
                continue;
            }
            while (!isEmpty(stack) && precedence(getTop(stack)) >= precedence(c)) {
                postfix[j++] = ' ';
                postfix[j++] = pop(stack);
            }
            // Push the operator to the stack
            push(stack, c);
            
        }

        // Case 2: c is open parenthesis
        else if (c == '(') {
            push(stack, c);
        }

        // Case 3: c is close parenthesis
        else if (c == ')') {
            // Case for negative numbers
            if (!isEmpty(isNegativeStack) && getTop(isNegativeStack)) {
				postfix[j++] = ' ';
				postfix[j++] = '-';
				pop(isNegativeStack);
			}

            // Pop all operators from the stack until an open parenthesis is met
            while (!isEmpty(stack) && getTop(stack) != '(') {
                postfix[j++] = ' ';
                postfix[j++] = pop(stack);
            }

            // Special case for negative numbers put in parentheses like (-5)
            if (isNegativeInParenthesis) {
                pop(stack);  // Remove the '('
                if (isOperator(getTop(stack))) {
					postfix[j++] = ' ';
					postfix[j++] = pop(stack);
				}
				isNegativeInParenthesis = false;
			}
            else pop(stack);  // Remove the '('

            // Case for negative numbers/expressions in parentheses
            if (!isEmpty(isNegativeParenthesis) && getTop(isNegativeParenthesis)) {
                postfix[j++] = ' ';
				postfix[j++] = '-';
				pop(isNegativeParenthesis);
            }
        }

        // Other cases
        else {
            if (c == ' ' && !isEmpty(isNegativeStack) && getTop(isNegativeStack)) {
                postfix[j++] = ' ';
                postfix[j++] = '-';
                pop(isNegativeStack);
            }

            if (c != ' ' ||
               !isOperator(infix[i + 1]) || 
               (isNum(infix[i + 2]) || infix[i + 2] == '(') ||
                c == ' ' && isOperator(infix[i - 1]) && infix[i + 2] == ' ')
            {
                postfix[j++] = c;
            }
            
            if (!isEmpty(isNegativeStack) && getTop(isNegativeStack) && infix[i + 1] == '\0') {
                postfix[j++] = ' ';
                postfix[j++] = '-';
                pop(isNegativeStack);  // Pop the top value from the stack
            }
        }
        i++;
    }

    while (!isEmpty(stack)) {
        postfix[j++] = ' ';
        postfix[j++] = pop(stack);
    }

    postfix[j] = '\0';  // Null-terminate the postfix string

    clear(stack);
    clear(isNegativeStack);
    clear(isNegativeParenthesis);
}


BigInt calculatePostfix(const char* postfix) {
    Stack<BigInt> stack;
    initStack(stack);

    int numSize = 0;
    int count = 0;
    int len = strlen(postfix);

    for (int i = 0; i < len; i++) {
        // count the maximum number of digits
        if (isNum(postfix[i])) {
			count++;
		}
        else {
            if (count > numSize) {
				numSize = count;
			}
            count = 0;
        }
    }
    if (count > numSize) {
        numSize = count;
    }

    if (numSize == 0) {
		return create("0");
	}

    // string to store the number
    char* num = new char[numSize + 1];
    count = 0;  // Reset the count

    for (int i = 0; i < len; i++) {
        if (postfix[i] != ' ' && !isOperator(postfix[i])) {
            num[count++] = postfix[i];
        }
        else if (postfix[i] == ' ' && count > 0) {
            num[count] = '\0';
            BigInt a = create(num);
            push(stack, a);
            count = 0;  // Reset the count for the next number
        }
        else if (isOperator(postfix[i])) {
            BigInt b = pop(stack);
            BigInt a = pop(stack);
            BigInt c;
            init(c);
            if (postfix[i] == '+') {
                c = a + b;
            }
            else if (postfix[i] == '-') {
                c = a - b;
            }
            else if (postfix[i] == '*') {
                c = a * b;
            }
            else if (postfix[i] == ':' || postfix[i] == '/') {
                c = a / b;
            }
            push(stack, c);
        }
    }

    BigInt result = pop(stack);
    delete[] num;
    clear(stack);

    return result;
}


int getMaxLenOfLines(const char* fileName) {
    FILE* file = fopen(fileName, "r");
    if (!file) {
        printf("Error opening file.\n");
        return -1;
    }

    int maxLen = 0;
    int len = 0;
    char buffer;

    while ((buffer = fgetc(file)) != EOF) {
        // Ex: -5 = 0 5 - => from 2 to 5 characters => +4
        if (buffer == '-') {
            len = len + 4;
        }
        else len++;
        if (buffer == '\n') {
            if (len > maxLen) {
                maxLen = len;
            }
            len = 0;
        }
    }

    // Check the last line if it doesn't end with \n
    if (len > maxLen) {
        maxLen = len;
    }

    fclose(file);
    return maxLen;
}


void run() {
    FILE* inFile = fopen("input.txt", "r");
    FILE* outFile = fopen("output.txt", "w");

    if (inFile == NULL) {
        printf("Cannot open file input.txt\n");
        return;
    }
    if (outFile == NULL) {
        printf("Cannot open file output.txt\n");
        return;
    }

    int maxLen = getMaxLenOfLines("input.txt");
    if (maxLen < 0) {
        return;
    }

    char* line = new char[maxLen + 1];
    char* postfix = new char[maxLen + 1];

    bool firstLine = true;
    while (fgets(line, maxLen + 1, inFile) != NULL) {
        int len = strlen(line);
        if (line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        infixToPostfix(line, postfix);
        cout << "Postfix: " << postfix << endl;

        BigInt result = calculatePostfix(postfix);
        char* resultStr = toString(result);

        if (firstLine) {
            fprintf(outFile, "%s", resultStr);
            firstLine = false;
        }
        else {
            fprintf(outFile, "\n%s", resultStr);
        }

        delete[] resultStr;

    }

    cout << "\nSuccessfully read input.txt and write to output.txt\n";
    delete[] postfix;
    delete[] line;

    fclose(inFile);
    fclose(outFile);
}
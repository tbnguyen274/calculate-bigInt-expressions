#define _CRT_SECURE_NO_WARNINGS
#include "CalculateBigInt.h"

int main() {
    FILE* inFile = fopen("input.txt", "r");
    FILE* outFile = fopen("output.txt", "w");

    if (inFile == NULL) {
        printf("Cannot open file input.txt\n");
        return 1;
    }
    if (outFile == NULL) {
        printf("Cannot open file output.txt\n");
        return 1;
    }

    int maxLen = getMaxLenOfLines("input.txt");
    if (maxLen < 0) {
        return 1;
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
        cout << "Postfix: " << postfix << endl << endl;

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

    cout << "Successfully read input.txt and write to output.txt\n";
    delete[] postfix;
    delete[] line;

    fclose(inFile);
    fclose(outFile);

    // Another way: run the program using run() in CalculateBigInt.cpp
    //run();

    return 0;
}
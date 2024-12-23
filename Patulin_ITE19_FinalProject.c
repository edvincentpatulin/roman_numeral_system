#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function declarations
int convertRomanToValue(char roman);
int convertRomanToDecimal(const char *roman);
void convertNumberToWords(int num, char *result);
void handleFileOperations(const char *inputFileName, const char *outputFileName);
void printResult(const char *firstOperand, int firstDecimal, const char *secondOperand, int secondDecimal, char operation, int finalResult, const char *wordRepresentation);

int main() {
    handleFileOperations("input.txt", "output.txt");
    return 0;
}

// Convert single Roman numeral to its decimal value
int convertRomanToValue(char roman) {
    switch (roman) {
        case 'I': return 1;
        case 'V': return 5;
        case 'X': return 10;
        case 'L': return 50;
        case 'C': return 100;
        case 'D': return 500;
        case 'M': return 1000;
        default: return 0;
    }
}

// Convert a Roman numeral string to a decimal number
int convertRomanToDecimal(const char *roman) {
    int sum = 0, previousValue = 0;

    for (int i = 0; roman[i] != '\0'; i++) {
        int currentValue = convertRomanToValue(roman[i]);

        if (currentValue == 0) return -1; // Invalid Roman numeral

        if (currentValue > previousValue) {
            sum += currentValue - 2 * previousValue; // Adjust for subtractive notation
        } else {
            sum += currentValue;
        }

        previousValue = currentValue;
    }

    return sum;
}

// Convert a number to its English word representation
void convertNumberToWords(int num, char *result) {
    const char *ones[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
    const char *teens[] = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    const char *tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
    const char *thousands[] = {"", "Thousand", "Million", "Billion"};

    char intermediateResult[200] = "";
    int groupIndex = 0;

    while (num > 0) {
        int segment = num % 1000;

        if (segment > 0) {
            char segmentResult[200] = "";

            if (segment / 100 > 0) {
                sprintf(segmentResult, "%s Hundred ", ones[segment / 100]);
            }

            if (segment % 100 >= 10 && segment % 100 < 20) {
                strcat(segmentResult, teens[segment % 100 - 10]);
            } else {
                strcat(segmentResult, tens[(segment % 100) / 10]);
                strcat(segmentResult, " ");
                strcat(segmentResult, ones[segment % 10]);
            }

            if (strlen(thousands[groupIndex]) > 0) {
                strcat(segmentResult, " ");
                strcat(segmentResult, thousands[groupIndex]);
            }

            strcat(segmentResult, " ");
            strcat(segmentResult, intermediateResult);
            strcpy(intermediateResult, segmentResult);
        }

        num /= 1000;
        groupIndex++;
    }

    strcpy(result, intermediateResult);
}

// Process input and output files
void handleFileOperations(const char *inputFileName, const char *outputFileName) {
    FILE *inputFile = fopen(inputFileName, "r");
    FILE *outputFile = fopen(outputFileName, "w");

    if (inputFile == NULL || outputFile == NULL) {
        printf("Error: Unable to open files.\n");
        exit(1);
    }

    char line[256];
    while (fgets(line, sizeof(line), inputFile)) {
        char firstOperand[100], secondOperand[100], wordRepresentation[200];
        char operation;
        int firstDecimal, secondDecimal, finalResult;

        // Parse input line
        if (sscanf(line, "%s %c %s", firstOperand, &operation, secondOperand) == 3) {
            firstDecimal = convertRomanToDecimal(firstOperand);
            secondDecimal = convertRomanToDecimal(secondOperand);

            if (firstDecimal == -1 || secondDecimal == -1) {
                fprintf(outputFile, "Invalid Roman numeral: %s\n", line);
                continue;
            }

            // Perform the specified operation
            switch (operation) {
                case '+': finalResult = firstDecimal + secondDecimal; break;
                case '-': finalResult = firstDecimal - secondDecimal; break;
                case '*': finalResult = firstDecimal * secondDecimal; break;
                case '/':
                    if (secondDecimal == 0) {
                        fprintf(outputFile, "Error: Division by zero: %s\n", line);
                        continue;
                    }
                    finalResult = firstDecimal / secondDecimal;
                    break;
                default:
                    fprintf(outputFile, "Invalid operator: %s\n", line);
                    continue;
            }

            // Generate word representation (use absolute value for negative results)
            if (finalResult < 0) {
                convertNumberToWords(-finalResult, wordRepresentation);
            } else {
                convertNumberToWords(finalResult, wordRepresentation);
            }

            fprintf(outputFile, "%s\n", wordRepresentation);

            // Display result to console
            printResult(firstOperand, firstDecimal, secondOperand, secondDecimal, operation, finalResult, wordRepresentation);
        } else {
            fprintf(outputFile, "Invalid input format: %s\n", line);
        }
    }

    fclose(inputFile);
    fclose(outputFile);
}

// Display the result on the console
void printResult(const char *firstOperand, int firstDecimal, const char *secondOperand, int secondDecimal, char operation, int finalResult, const char *wordRepresentation) {
    printf("%s (%d) %c %s (%d) = %d (%s)\n", firstOperand, firstDecimal, operation, secondOperand, secondDecimal, finalResult, wordRepresentation);
}

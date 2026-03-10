#include "CSVPrettyPrinter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

void trimTrailingWhitespace(char* str)
{
    int len = strlen(str);
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t' || str[len - 1] == '\n' || str[len - 1] == '\r')) {
        str[len - 1] = '\0';
        len--;
    }
}

int areFilesEqual(char* file1, char* file2)
{
    FILE* f1 = fopen(file1, "r");
    FILE* f2 = fopen(file2, "r");

    if (f1 == NULL || f2 == NULL) {
        if (f1 != NULL)
            fclose(f1);
        if (f2 != NULL)
            fclose(f2);
        return 0;
    }

    char line1[MAX_LINE];
    char line2[MAX_LINE];
    char trimmed1[MAX_LINE];
    char trimmed2[MAX_LINE];
    int lineNum = 1;

    char* result1 = fgets(line1, MAX_LINE, f1);
    char* result2 = fgets(line2, MAX_LINE, f2);

    while (result1 != NULL && result2 != NULL) {
        strcpy(trimmed1, line1);
        strcpy(trimmed2, line2);
        trimTrailingWhitespace(trimmed1);
        trimTrailingWhitespace(trimmed2);

        if (strcmp(trimmed1, trimmed2) != 0) {
            printf("  Несовпадение в строке %d:\n", lineNum);
            printf("  Ожидалось: %s", line2);
            printf("  Получено:  %s", line1);
            fclose(f1);
            fclose(f2);
            return 0;
        }

        result1 = fgets(line1, MAX_LINE, f1);
        result2 = fgets(line2, MAX_LINE, f2);
        lineNum++;
    }

    if (result1 != NULL || result2 != NULL) {
        printf("  Файлы разной длины (строка %d)\n", lineNum);
        fclose(f1);
        fclose(f2);
        return 0;
    }

    fclose(f1);
    fclose(f2);
    return 1;
}

void runTest(char* testName)
{
    printf("\n Тест %s \n", testName);

    char inputFile[100];
    char expectedFile[100];

    sprintf(inputFile, "%sTestInput.csv", testName);
    sprintf(expectedFile, "%sTestOutput.txt", testName);

    char* argv[3];
    argv[0] = "csvPrinter";
    argv[1] = inputFile;
    argv[2] = NULL;

    csvPrinterMain(2, argv);

    if (areFilesEqual("output.txt", expectedFile)) {
        printf(" ТЕСТ ПРОЙДЕН\n");
    } else {
        printf(" ТЕСТ НЕ ПРОЙДЕН\n");
    }
}

int main()
{
    printf("Запуск тестов...\n");

    runTest("1");
    runTest("2");
    runTest("3");
    runTest("4");

    printf("\nТесты завершены.\n");

    return 0;
}
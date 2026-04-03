#include "CSVPrettyPrinter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int countColumns(const char* str)
{
    int countCol = 1;
    for (int i = 0; str[i] != '\0' && str[i] != '\n' && str[i] != '\r'; i++) {
        if (str[i] == ',') {
            countCol++;
        }
    }
    return countCol;
}

void changeCRLF(char* str)
{
    str[strcspn(str, "\r\n")] = '\0';
}

void updateMax(char* str, int* maxColSize)
{
    int index = 0;
    int numberCol = 0;
    changeCRLF(str);
    while (str[index] != '\0') {
        int count = 0;
        while (str[index] != '\0' && str[index] != ',') {
            count++;
            index++;
        }
        if (count > maxColSize[numberCol]) {
            maxColSize[numberCol] = count;
        }
        numberCol++;

        if (str[index] == ',')
            index++;
    }
}

void separatorPrint(FILE* out, const int* maxColLen, int countCol, char symbol)
{
    for (int i = 0; i < countCol; i++) {
        fprintf(out, "+");
        // +2 добавляется для учета созданных отступов (по одному пробелу) слева и справа от текста ячейки
        for (int j = 0; j < maxColLen[i] + 2; j++) {
            fprintf(out, "%c", symbol);
        }
    }
    fprintf(out, "+\n");
}

int isEnd(const char* str, int i)
{
    if (str[i] == '\0')
        return 1;
    if (str[i] == '\n')
        return 1;
    if (str[i] == '\r')
        return 1;
    if (str[i] == ',')
        return 1;
    return 0;
}

int findCellLen(const char* str, int start)
{
    int i = start;
    // Пока НЕ конец ячейки — идём дальше
    while (!isEnd(str, i)) {
        i++;
    }
    int cellLen = i - start;
    return cellLen;
}

int isCellNumber(char* cell, int cellLen)
{
    int cellIsNumber = 1;
    int dotCounter = 0;

    if (cellLen == 0) {
        cellIsNumber = 0;
    } else {
        for (int j = 0; j < cellLen; j++) {
            if (j == 0 && cell[j] == '-') {
                continue;
            } else if (cell[j] == '.') {
                dotCounter++;
                if (dotCounter > 1) {
                    cellIsNumber = 0;
                }
            } else if (cell[j] < '0' || cell[j] > '9') {
                cellIsNumber = 0;
            }
        }
    }
    return cellIsNumber;
}

int printOneCell(FILE* out, const int* maxColLen, const char* str, int start, int colIndex)
{
    int cellLen = findCellLen(str, start);
    char* cell = (char*)calloc(cellLen + 1, 1);
    if (cell == NULL) {
        printf("Ошибка: не удалось выделить память для ячейки\n");
        return start;
    }

    for (int k = 0; k < cellLen; k++) {
        cell[k] = str[start + k];
    }
    int isNumber = isCellNumber(cell, cellLen);

    if (isNumber == 1) {
        fprintf(out, "| %*s ", maxColLen[colIndex], cell);
    } else {
        fprintf(out, "| %-*s ", maxColLen[colIndex], cell);
    }

    free(cell);
    return cellLen + start;
}

FILE* openInputFile(int argc, char* argv[])
{
    char* inputFileName = "input.csv"; // значение по умолчанию

    // если передан аргумент, используем его
    if (argc > 1) {
        inputFileName = argv[1];
    }

    FILE* inputFile = fopen(inputFileName, "r");
    return inputFile;
}

int getFileSize(FILE* inputFile)
{
    fseek(inputFile, 0, SEEK_END);
    long sizeLong = ftell(inputFile);
    if (sizeLong <= 0) {
        return 0;
    }
    int size = (int)sizeLong;
    size++;
    fseek(inputFile, 0, SEEK_SET);
    return size;
}

int getColLens(FILE* inputFile, int size, int** maxColLen, int* countCol)
{
    char* buffer = (char*)malloc(sizeof(char) * size);
    if (buffer == NULL) {
        return 1;
    }

    fgets(buffer, size, inputFile);
    *countCol = countColumns(buffer);

    *maxColLen = (int*)calloc(*countCol, sizeof(int));
    if (*maxColLen == NULL) {
        free(buffer);
        return 1;
    }

    updateMax(buffer, *maxColLen);

    while (fgets(buffer, size, inputFile) != NULL) {
        updateMax(buffer, *maxColLen);
    }

    fseek(inputFile, 0, SEEK_SET);
    free(buffer);
    return 0;
}

int printHeader(FILE* outputFile, FILE* inputFile, int size, int* maxColLen, int countCol)
{
    char* buffer = (char*)malloc(sizeof(char) * (size + 1));
    fgets(buffer, size, inputFile);
    changeCRLF(buffer);

    separatorPrint(outputFile, maxColLen, countCol, '=');

    int currentPosition = 0;
    for (int columnIndex = 0; columnIndex < countCol; columnIndex++) {
        int cellStart = currentPosition;

        // Пока НЕ конец ячейки
        while (!isEnd(buffer, currentPosition)) {
            currentPosition++;
        }

        int cellLength = currentPosition - cellStart;
        char* cellContent = (char*)calloc(cellLength + 1, 1);
        if (cellContent == NULL) {
            printf("Ошибка: не удалось выделить память\n");
            free(buffer);
            return 1;
        }

        for (int charIndex = 0; charIndex < cellLength; charIndex++) {
            cellContent[charIndex] = buffer[cellStart + charIndex];
        }

        fprintf(outputFile, "| %-*s ", maxColLen[columnIndex], cellContent);
        free(cellContent);

        if (buffer[currentPosition] == ',') {
            currentPosition++;
        }
    }
    fprintf(outputFile, "|\n");

    separatorPrint(outputFile, maxColLen, countCol, '=');

    free(buffer);
    return 0;
}

int printCells(FILE* outputFile, FILE* inputFile, int size, int* maxColLen, int countCol)
{
    char* buffer = (char*)malloc(sizeof(char) * (size + 1));
    while (fgets(buffer, size, inputFile)) {
        changeCRLF(buffer);

        int currentPosition = 0;
        for (int columnIndex = 0; columnIndex < countCol; columnIndex++) {
            currentPosition = printOneCell(outputFile, maxColLen, buffer, currentPosition, columnIndex);

            if (buffer[currentPosition] == ',') {
                currentPosition++;
            }
        }
        fprintf(outputFile, "|\n");

        separatorPrint(outputFile, maxColLen, countCol, '-');
    }

    free(buffer);
    return 0;
}

int csvPrinterMain(int argc, char* argv[])
{
    FILE* inputFile = openInputFile(argc, argv);
    if (inputFile == NULL) {
        printf("Ошибка: не удалось открыть файл\n");
        return 1;
    }

    FILE* outputFile = fopen("output.txt", "w");
    if (outputFile == NULL) {
        fclose(inputFile);
        return 1;
    }

    int size = getFileSize(inputFile);
    if (size <= 0) {
        printf("Ошибка: файл пустой или поврежден\n");
        fclose(inputFile);
        fclose(outputFile);
        return 1;
    }

    int* maxColLen;
    int countCol;
    int error = getColLens(inputFile, size, &maxColLen, &countCol);
    if (error == 1) {
        printf("Ошибка: не удалось выделить память\n");
        fclose(inputFile);
        fclose(outputFile);
        return 1;
    }

    // Печать заголовка
    error = printHeader(outputFile, inputFile, size, maxColLen, countCol);
    if (error != 0) {
        free(maxColLen);
        fclose(inputFile);
        fclose(outputFile);
        return 1;
    }

    // Печать остальных ячеек
    error = printCells(outputFile, inputFile, size, maxColLen, countCol);
    if (error != 0) {
        free(maxColLen);
        fclose(inputFile);
        fclose(outputFile);
        return 1;
    }

    free(maxColLen);
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
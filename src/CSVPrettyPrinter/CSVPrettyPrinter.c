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

void updateMax(char* str, int* maxColSize)
{
    int index = 0;
    int numberCol = 0;
    str[strcspn(str, "\r\n")] = '\0';
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
        for (int j = 0; j < maxColLen[i] + 2; j++) {
            fprintf(out, "%c", symbol);
        }
    }
    fprintf(out, "+\n");
}

int oneCell(FILE* out, const int* maxColLen, const char* str, int start, int colIndex)
{
    int i = start;
    while (str[i] != '\0' && str[i] != '\n' && str[i] != '\r' && str[i] != ',') {
        i++;
    }
    int cellLen = i - start;

    char* cell = (char*)malloc((cellLen + 1) * sizeof(char));
    if (cell == NULL) {
        printf("Ошибка: не удалось выделить память для ячейки\n");
        return start;
    }

    for (int k = 0; k < cellLen; k++) {
        cell[k] = str[start + k];
    }
    cell[cellLen] = '\0';

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

    if (cellIsNumber == 1) {
        fprintf(out, "| %*s ", maxColLen[colIndex], cell);
    } else {
        fprintf(out, "| %-*s ", maxColLen[colIndex], cell);
    }

    free(cell);
    return i;
}

int csvPrinterMain(int argc, char* argv[])
{
    char* inputFileName = "input.csv"; // значение по умолчанию

    // если передан аргумент, используем его
    if (argc > 1) {
        inputFileName = argv[1];
    }

    // теперь открываем переданный файл
    FILE* iFile = fopen(inputFileName, "r");
    if (iFile == NULL) {
        printf("Ошибка: не удалось открыть %s\n", inputFileName);
        return 1;
    }
    FILE* oFile = fopen("output.txt", "w");
    if (oFile == NULL) {
        fclose(iFile);
        return 1;
    }

    fseek(iFile, 0, SEEK_END);
    long sizeLong = ftell(iFile);
    if (sizeLong <= 0) {
        printf("Ошибка: файл пустой или поврежден\n");
        fclose(iFile);
        fclose(oFile);
        return 1;
    }
    int size = (int)sizeLong;
    size++;
    fseek(iFile, 0, SEEK_SET);

    char* buffer = (char*)malloc(sizeof(char) * (size + 1));
    if (buffer == NULL) {
        printf("Ошибка: не удалось выделить память для буфера\n");
        fclose(iFile);
        fclose(oFile);
        return 1;
    }

    fgets(buffer, size, iFile);
    int countCol = countColumns(buffer);

    int* maxColLen = (int*)calloc(countCol, sizeof(int));
    if (maxColLen == NULL) {
        printf("Ошибка: не удалось выделить память\n");
        free(buffer);
        fclose(iFile);
        fclose(oFile);
        return 1;
    }

    updateMax(buffer, maxColLen);

    while (fgets(buffer, size, iFile) != NULL) {
        updateMax(buffer, maxColLen);
    }

    // вот тут заголовок
    fseek(iFile, 0, SEEK_SET);
    fgets(buffer, size, iFile);
    buffer[strcspn(buffer, "\r\n")] = '\0';

    separatorPrint(oFile, maxColLen, countCol, '=');

    int currentPosition = 0;
    for (int columnIndex = 0; columnIndex < countCol; columnIndex++) {
        int cellStart = currentPosition;

        // конец ячейки
        while (buffer[currentPosition] != '\0' && buffer[currentPosition] != ',' && buffer[currentPosition] != '\n' && buffer[currentPosition] != '\r') {
            currentPosition++;
        }

        int cellLength = currentPosition - cellStart;
        char* cellContent = (char*)malloc(cellLength + 1);
        if (cellContent == NULL) {
            printf("Ошибка: не удалось выделить память\n");
            free(buffer);
            free(maxColLen);
            fclose(iFile);
            fclose(oFile);
            return 1;
        }

        for (int charIndex = 0; charIndex < cellLength; charIndex++) {
            cellContent[charIndex] = buffer[cellStart + charIndex];
        }
        cellContent[cellLength] = '\0';

        fprintf(oFile, "| %-*s ", maxColLen[columnIndex], cellContent);
        free(cellContent);

        if (buffer[currentPosition] == ',') {
            currentPosition++;
        }
    }
    fprintf(oFile, "|\n");

    separatorPrint(oFile, maxColLen, countCol, '=');

    // другие ячейки
    while (fgets(buffer, size, iFile)) {
        buffer[strcspn(buffer, "\r\n")] = '\0';

        currentPosition = 0;
        for (int columnIndex = 0; columnIndex < countCol; columnIndex++) {
            currentPosition = oneCell(oFile, maxColLen, buffer, currentPosition, columnIndex);

            if (buffer[currentPosition] == ',') {
                currentPosition++;
            }
        }
        fprintf(oFile, "|\n");

        separatorPrint(oFile, maxColLen, countCol, '-');
    }

    free(buffer);
    free(maxColLen);

    fclose(iFile);
    fclose(oFile);

    return 0;
}
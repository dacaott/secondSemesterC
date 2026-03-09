#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int countColumns(char* str)
{
    int countCol = 1;
    for (int i = 0; str[i] != '\0'; i++) {
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
    while (str[index] != '\0' && str[index] != '\n') {
        int count = 0;
        while (str[index] != '\0' && str[index] != '\n' && str[index] != ',') {
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

void separatorPrint(FILE* out, int* maxColLen, int countCol, char symbol)
{
    for (int i = 0; i < countCol; i++) {
        fprintf(out, "+");
        for (int j = 0; j < maxColLen[i] + 2; j++) {
            fprintf(out, "%c", symbol);
        }
    }
    fprintf(out, "+\n");
}

int oneCell(FILE* out, int* maxColLen, char* str, int start, int countCol)
{
    int cellLen = 0;
    for (int i = start; str[i] != '\0' && str[i] != '\n' && str[i] != ','; i++) {
        cellLen++;
    }

    char* cell = (char*)calloc(cellLen + 1, sizeof(char));
    for (int k = 0; k < cellLen; k++) {
        cell[k] = str[start + k];
    }
    cell[cellLen] = '\0';

    int cellIsNumber = 1;
    int dotCounter = 0;
    for (int j = 0; j < cellLen; j++) {
        if (j != 0 && cell[j] == '-') {
            cellIsNumber = 0;
        } else if (cell[j] == '.') {
            dotCounter++;
            if (dotCounter > 1) {
                cellIsNumber = 0;
            }
        } else if (cell[j] < '0' || cell[j] > '9') {
            cellIsNumber = 0;
        }
    }

    if (cellIsNumber == 1) {
        fprintf(out, "| %*s ", maxColLen[countCol], cell);
    } else {
        fprintf(out, "| %-*s ", maxColLen[countCol], cell);
    }

    free(cell);
    return cellLen + start;
}

int main()
{

    FILE* iFile = fopen("input.csv", "r");
    FILE* oFile = fopen("output.txt", "w");

    fseek(iFile, 0, SEEK_END);
    int size = ftell(iFile);
    fseek(iFile, 0, SEEK_SET);

    char* buffer = (char*)malloc(sizeof(char) * size);

    fgets(buffer, size, iFile);
    int countCol = countColumns(buffer);

    int* maxColLen = (int*)calloc(countCol, sizeof(int));

    updateMax(buffer, maxColLen);

    while (fgets(buffer, size, iFile)) {
        updateMax(buffer, maxColLen);
    }

    // вот тут заголовок
    fseek(iFile, 0, SEEK_SET);
    fgets(buffer, size, iFile);
    buffer[strcspn(buffer, "\n")] = '\0';

    separatorPrint(oFile, maxColLen, countCol, '=');

    int currentPosition = 0;
    for (int columnIndex = 0; columnIndex < countCol; columnIndex++) {
        int cellStart = currentPosition;

        // конец ячейки
        while (buffer[currentPosition] != '\0' && buffer[currentPosition] != ',' && buffer[currentPosition] != '\n') {
            currentPosition++;
        }

        int cellLength = currentPosition - cellStart;
        char* cellContent = (char*)malloc(cellLength + 1);
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
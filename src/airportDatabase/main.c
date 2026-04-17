#include "cmdUtils.h"
#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
    if (!isValidArgs(argv, argc)) {
        return 1;
    }

    FILE* iFile = fopen(argv[1], "r");
    if (iFile == NULL) {
        fprintf(stderr, "Could not open file: %s\n", argv[1]);
        return 1;
    }

    struct Node* root = NULL;
    root = buildTree(iFile);
    fclose(iFile);

    char command[1000];
    bool Run = true;

    while (Run) {
        fputs("Enter command (add/find/delete/save/quit): ", stdout);
        fgets(command, 1000, stdin);

        if (strcmp(command, "quit\n") == 0) {
            Run = false;
        } else {
            char cmd[10];
            char cmd_value[1000] = "";

            sscanf(command, "%s %[^\n]", cmd, cmd_value);

            if (strcmp(cmd, "add") == 0) {
                char key[4];
                char value[1000];
                if (sscanf(cmd_value, "%[^:]:%[^\n]", key, value) == 2) {
                    root = insert(root, key, value);
                } else {
                    printf("Invalid add format. Use KEY:Value\n");
                }
            } else if (strcmp(cmd, "find") == 0) {
                struct Node* result = find(root, cmd_value);
                if (result != NULL) {
                    printf("%s -> %s\n", result->key, result->value);
                } else {
                    printf("IATA code not found in database\n");
                }
            } else if (strcmp(cmd, "delete") == 0) {
                root = deleteNode(root, cmd_value);
            } else if (strcmp(cmd, "save") == 0) {
                FILE* oFile = fopen(argv[1], "w");
                if (oFile == NULL) {
                    fprintf(stderr, "Could not open file: %s\n", argv[1]);
                } else {
                    saveTree(root, oFile);
                    fclose(oFile);
                    printf("Saved successfully.\n");
                }
            } else {
                fprintf(stderr, "Unknown command: %s\n", cmd);
            }
        }
    }

    deleteTree(root);
    return 0;
}
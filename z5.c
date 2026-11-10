#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

typedef struct Stack {
    int data;
    struct Stack* next;
} Stack;

int Push(int data, Stack* head) {
    Stack* newNode = (Stack*)malloc(sizeof(Stack));
    if (!newNode) {
        puts("Greska pri alokaciji memorije!");
        return -1;
    }

    newNode->data = data;
    newNode->next = head->next;
    head->next = newNode;
    return 0;
}

int Pop(Stack* head, int* data) {
    if (!head->next) {
        puts("Stog je prazan!");
        return -1;
    }

    Stack* temp = head->next;
    *data = temp->data;
    head->next = temp->next;
    free(temp);
    return 0;
}

int DeleteStack(Stack* head) {
    while (head->next) {
        Stack* temp = head->next;
        head->next = temp->next;
        free(temp);
    }

    return 0;
}

int IsNumber(const char* str) {
    if (str[0] == '\0')
        return 0;

    int i = (str[0] == '+' || str[0] == '-') ? 1 : 0;
    if (str[i] == '\0')
        return 0;

    for (; str[i] != '\0'; i++) {
        if (!isdigit(str[i]))
            return 0;
    }

    return 1;
}

int IsOperator(const char* str) {
    return strcmp(str, "+") == 0 || strcmp(str, "-") == 0 ||
           strcmp(str, "*") == 0 || strcmp(str, "/") == 0;
}

int EvaluatePostfix(const char* fileName, Stack* head, int* result) {
    if (head->next)
        DeleteStack(head);

    FILE* fp = fopen(fileName, "r");
    if (!fp) {
        puts("Greska pri otvaranju datoteke!");
        return -1;
    }

    char buffer[MAX];
    while (fscanf(fp, "%99s", buffer) == 1) {
        if (IsOperator(buffer)) {
            int right, left, value;

            if (Pop(head, &right) != 0 || Pop(head, &left) != 0) {
                puts("Postfix izraz je u neispravnom formatu!");
                DeleteStack(head);
                fclose(fp);
                return -1;
            }

            switch (buffer[0]) {
            case '+':
                value = left + right;
                break;
            case '-':
                value = left - right;
                break;
            case '*':
                value = left * right;
                break;
            case '/':
                if (right == 0) {
                    puts("Dijeljenje s nulom nije moguce!");
                    DeleteStack(head);
                    fclose(fp);
                    return -1;
                }
                value = left / right;
                break;
            }

            if (Push(value, head) != 0) {
                DeleteStack(head);
                fclose(fp);
                return -1;
            }
        }
        else if (IsNumber(buffer)) {
            int value = atoi(buffer);
            if (Push(value, head) != 0) {
                DeleteStack(head);
                fclose(fp);
                return -1;
            }
        }
        else {
            puts("Postfix izraz je u neispravnom formatu!");
            DeleteStack(head);
            fclose(fp);
            return -1;
        }
    }

    if (ferror(fp)) {
        puts("Greska pri citanju datoteke!");
        DeleteStack(head);
        fclose(fp);
        return -1;
    }
    fclose(fp);

    if (!head->next) {
        puts("Postfix izraz je prazan ili neispravan!");
        return -1;
    }
    if (head->next->next) {
        puts("Ostalo je previse operanada na stogu, neispravan format postfix izraza!");
        DeleteStack(head);
        return -1;
    }

    return Pop(head, result);
}

int main(void) {
    Stack head = { 0 };
    int result;

    if (EvaluatePostfix("postfix.txt", &head, &result) == 0) {
        printf("Rjesenje je: %d\n", result);
    }
    else {
        puts("Doslo je do greske pri evaluaciji postfix izraza!");
    }

    DeleteStack(&head);
    return 0;
}

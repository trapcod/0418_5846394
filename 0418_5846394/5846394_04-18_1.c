#include <stdio.h>
#include <stdlib.h>
#include <string.h> // strlen �Լ� ����� ���� �߰�
#define MAX_STACK_SIZE 100
#define MAX_SIZE 100

typedef int element; // ���ÿ� ����� ����� Ÿ���� int�� ����

typedef struct {
    element stack[MAX_STACK_SIZE]; // ��Ҹ� ������ �迭
    int top; // ������ ����⸦ ����Ű�� �ε���
} StackType;

// ���� �ʱ�ȭ �Լ�
void init_stack(StackType* s) {
    s->top = -1;
}

// ���� ���� ���� �Լ�
int is_empty(StackType* s) {
    return (s->top == -1);
}

// ��ȭ ���� ���� �Լ�
int is_full(StackType* s) {
    return (s->top == (MAX_STACK_SIZE - 1));
}

// ���� �Լ�
void push(StackType* s, element item) {
    if (is_full(s)) {
        fprintf(stderr, "���� ��ȭ ����\n");
        return;
    }
    else s->stack[++(s->top)] = item;
}

element pop(StackType* s) {
    if (is_empty(s)) {
        fprintf(stderr, "���� ���� ����\n");
        exit(1); // ������ ����
    }
    else return s->stack[(s->top)--];
}

int prec(char op) {
    switch (op) {
    case '(': case ')': return 0;
    case '+': case '-': return 1;
    case '*': case '/': return 2;
    }
    return -1;
}

element peek(StackType* s) {
    if (is_empty(s)) {
        fprintf(stderr, "���� ���� ����\n");
        exit(1);
    }
    return s->stack[s->top];
}

int eval(char exp[]) {
    int op1, op2, value, i = 0;
    int len = (int)strlen(exp);
    char ch;
    StackType s;

    init_stack(&s);
    for (i = 0; i < len; i++) {
        ch = exp[i];
        printf("expression : %c\n", exp[i]);
        if (ch >= '0' && ch <= '9') { // �Է��� ������ ���
            value = ch - '0'; // char�� int�� ��ȯ
            push(&s, value);
        }
        else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') { // �������� ���
            op2 = pop(&s);
            op1 = pop(&s);
            switch (ch) {
            case '+': push(&s, op1 + op2); break;
            case '-': push(&s, op1 - op2); break;
            case '*': push(&s, op1 * op2); break;
            case '/': push(&s, op1 / op2); break;
            }
        }
        // ����� ���� ���ڸ� ����
        else if (ch == ' ' || ch == '\n') {
            continue;
        }
    }
    return pop(&s);
}

void infix_to_postfix(char exp[], char result[]) {
    int i = 0, j = 0;
    char ch, top_op;
    int len = (int)strlen(exp);
    StackType s;

    init_stack(&s);
    for (i = 0; i < len; i++) {
        ch = exp[i];
        switch (ch) {
        case '+': case '-': case '*': case '/':
            while (!is_empty(&s) && (prec(ch) <= prec(peek(&s))))
                result[j++] = pop(&s);
            push(&s, ch);
            break;
        case '(':
            push(&s, ch);
            break;
        case ')':
            top_op = pop(&s);
            while (top_op != '(') {
                result[j++] = top_op;
                top_op = pop(&s);
            }
            break;
        default:
            if (ch >= '0' && ch <= '9')  // �ǿ������� ���
                result[j++] = ch;
            break;
        }
    }
    while (!is_empty(&s))
        result[j++] = pop(&s);
    result[j] = '\0';  // ���ڿ��� ���� ��Ÿ���� NULL ���� ����
}

int main(void) {
    char infixexpression[MAX_SIZE] = ""; // �ʱ�ȭ �߰�
    char postfixexpression[MAX_SIZE] = ""; // �ʱ�ȭ �߰�
    int result;
    while (1) {
        printf("������ ���� �޴��� �����ϴ� ���α׷�: \n");
        printf("1. �������� �Է¹���\n");
        printf("2. �������� ���������� ��ȯ\n");
        printf("3. �������� ���\n");
        printf("4. ����\n");
        printf("�޴��� �����ϼ���: ");

        int choice;
        scanf("%d", &choice);
        getchar();

        switch (choice) {
        case 1:
            printf("�������� �Է��ϼ���: ");
            fgets(infixexpression, sizeof(infixexpression), stdin);
            printf("�Էµ� ������ : %s\n", infixexpression);
            break;
        case 2:
            infix_to_postfix(infixexpression, postfixexpression);
            printf("����ǥ�ü��� : %s\n", postfixexpression);
            break;
        case 3:
            result = eval(postfixexpression);
            printf("����� : %d\n", result);
            break;
        case 4:
            printf("���α׷��� �����մϴ�.\n");
            return 0;
        default:
            printf("�߸��� �޴� �����Դϴ�. �ٽ� �����ϼ���.\n");
            break;
        }
    }
}
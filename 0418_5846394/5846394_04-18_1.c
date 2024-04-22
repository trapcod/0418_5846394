#include <stdio.h>
#include <stdlib.h>
#include <string.h> // strlen 함수 사용을 위해 추가
#define MAX_STACK_SIZE 100
#define MAX_SIZE 100

typedef int element; // 스택에 저장될 요소의 타입을 int로 변경

typedef struct {
    element stack[MAX_STACK_SIZE]; // 요소를 저장할 배열
    int top; // 스택의 꼭대기를 가리키는 인덱스
} StackType;

// 스택 초기화 함수
void init_stack(StackType* s) {
    s->top = -1;
}

// 공백 상태 검출 함수
int is_empty(StackType* s) {
    return (s->top == -1);
}

// 포화 상태 검출 함수
int is_full(StackType* s) {
    return (s->top == (MAX_STACK_SIZE - 1));
}

// 삽입 함수
void push(StackType* s, element item) {
    if (is_full(s)) {
        fprintf(stderr, "스택 포화 에러\n");
        return;
    }
    else s->stack[++(s->top)] = item;
}

element pop(StackType* s) {
    if (is_empty(s)) {
        fprintf(stderr, "스택 공백 에러\n");
        exit(1); // 비정상 종료
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
        fprintf(stderr, "스택 공백 에러\n");
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
        if (ch >= '0' && ch <= '9') { // 입력이 숫자인 경우
            value = ch - '0'; // char를 int로 변환
            push(&s, value);
        }
        else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') { // 연산자인 경우
            op2 = pop(&s);
            op1 = pop(&s);
            switch (ch) {
            case '+': push(&s, op1 + op2); break;
            case '-': push(&s, op1 - op2); break;
            case '*': push(&s, op1 * op2); break;
            case '/': push(&s, op1 / op2); break;
            }
        }
        // 공백과 개행 문자를 무시
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
            if (ch >= '0' && ch <= '9')  // 피연산자인 경우
                result[j++] = ch;
            break;
        }
    }
    while (!is_empty(&s))
        result[j++] = pop(&s);
    result[j] = '\0';  // 문자열의 끝을 나타내는 NULL 문자 삽입
}

int main(void) {
    char infixexpression[MAX_SIZE] = ""; // 초기화 추가
    char postfixexpression[MAX_SIZE] = ""; // 초기화 추가
    int result;
    while (1) {
        printf("다음과 같은 메뉴로 동작하는 프로그램: \n");
        printf("1. 중위식을 입력받음\n");
        printf("2. 중위식을 후위식으로 변환\n");
        printf("3. 후위식을 계산\n");
        printf("4. 종료\n");
        printf("메뉴를 선택하세요: ");

        int choice;
        scanf("%d", &choice);
        getchar();

        switch (choice) {
        case 1:
            printf("중위식을 입력하세요: ");
            fgets(infixexpression, sizeof(infixexpression), stdin);
            printf("입력된 중위식 : %s\n", infixexpression);
            break;
        case 2:
            infix_to_postfix(infixexpression, postfixexpression);
            printf("후위표시수식 : %s\n", postfixexpression);
            break;
        case 3:
            result = eval(postfixexpression);
            printf("계산결과 : %d\n", result);
            break;
        case 4:
            printf("프로그램을 종료합니다.\n");
            return 0;
        default:
            printf("잘못된 메뉴 선택입니다. 다시 선택하세요.\n");
            break;
        }
    }
}
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


struct stack {
    unsigned len;
    int top;
    int* items;
};

typedef struct stack pila;

void createEmpty(pila *p) {
    p->top = -1;
}

struct stack* newStack(unsigned capacity) {
    struct stack *p = (struct stack*)malloc(sizeof(struct stack));
    if (!p)
        return NULL;
 
    p->len = capacity;
    p->top = -1;
    //p->items = (int*)malloc(sizeof(int) * capacity);
    p->items = (int*)malloc(p->len * sizeof(int));
    return p;
}


void push(struct stack* p, char c) {
    p->top++;
    p->items[p->top] = c;
}

int isEmpty(struct stack* p) {
    return p->top == -1;
}

char pop(struct stack *p) {
    if (!isEmpty(p))
        return p->items[p->top--];
    return '$';
}

char peek(struct stack* p) {
    return p->items[p->top];
}

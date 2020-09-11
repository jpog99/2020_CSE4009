//Template
#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
    int nData;
    struct Node *next;
}Node;

typedef struct Stack{
    Node *top;
}Stack;

void InitializeStack(Stack *stack);
void Push(Stack *stack, int nData);
int Pop(Stack *stack);


int main(void){
    Stack *s;
    InitializeStack(&s);

    Push(&s,10);
    Push(&s,20);
    Pop(&s);
    Pop(&s);
}

void InitializeStack(Stack *stack){
    stack = (Stack*)malloc(sizeof(Stack));
    stack->top = NULL;
}

void Push(Stack *stack, int nData){
    Node* new_node = (Node*) malloc(sizeof(Node));;
    new_node->nData = nData;
    if(stack->top == NULL){
        stack->top = new_node;
        new_node->next = NULL;
    }else{
        new_node->next = stack->top;
        stack->top = new_node;
    }
}

int Pop(Stack *stack){
    if(stack->top != NULL){
        Node* oldtop = stack->top;
        int popped_data = oldtop->nData;
        stack->top = oldtop->next;
        free(oldtop);
        printf("Popped %d!\n",popped_data);
        return popped_data;
    }else{
        printf("Stack is empty!\n");
        free(stack);
        exit(0);
    }
}


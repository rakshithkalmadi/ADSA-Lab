#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100

// Stack structure
struct Stack {
    int array[MAX_SIZE];
    int top;
};

// Function to initialize an empty stack
void InitializeStack(struct Stack *S) {
    S->top = -1;
}

// Function to check if the stack is empty
int StackEmpty(struct Stack *S) {
    return S->top == -1;
}

// Function to check if the stack is full
int StackFull(struct Stack *S) {
    return S->top == MAX_SIZE - 1;
}

// Function to push an element onto the stack
void Push(struct Stack *S, int x, int *amortizedCost) {
    if (StackFull(S)) {
        printf("Stack overflow\n");
    } else {
        S->top++;
        S->array[S->top] = x;
        (*amortizedCost)++; // Increment the amortized cost
    }
}

// Function to pop an element from the stack
int Pop(struct Stack *S, int *amortizedCost) {
    if (StackEmpty(S)) {
        printf("Stack underflow\n");
        return -1;
    } else {
        int poppedElement = S->array[S->top];
        S->top--;
        (*amortizedCost)++; // Increment the amortized cost
        return poppedElement;
    }
}

// Function to pop k elements from the stack
void Multipop(struct Stack *S, int k, int *amortizedCost) {
    if (k <= 0) {
        return; // No elements to pop
    }

    int i;
    for (i = 0; i < k; i++) {
        if (!StackEmpty(S)) {
            S->top--;
            (*amortizedCost)++; // Increment the amortized cost for each pop
        } else {
            printf("Stack underflow\n");
            break; // Break if the stack is empty before popping k elements
        }
    }
}

int main() {
    struct Stack myStack;
    InitializeStack(&myStack);
    int amortizedCost = 0;

    // Perform a sequence of operations
    Push(&myStack, 10, &amortizedCost);
    Push(&myStack, 20, &amortizedCost);
    Push(&myStack, 30, &amortizedCost);
    Pop(&myStack, &amortizedCost);
    Multipop(&myStack, 2, &amortizedCost);

    printf("Amortized cost of operations: %d\n", amortizedCost);

    return 0;
}

#include <stdio.h>
#include <stdbool.h>

#define MAX_SIZE 100 // Maximum capacity of the stack

// Stack structure
struct Stack {
    int array[MAX_SIZE];
    int top;
};

// Function to initialize an empty stack
void InitializeStack(struct Stack *S) {
    S->top = -1; // Initialize the top to -1 to indicate an empty stack
}

// Function to check if the stack is empty
bool StackEmpty(struct Stack *S) {
    return S->top == -1;
}

// Function to check if the stack is full
bool StackFull(struct Stack *S) {
    return S->top == MAX_SIZE - 1;
}

// Function to push an element onto the stack
void Push(struct Stack *S, int x) {
    if (StackFull(S)) {
        printf("Stack overflow\n");
    } else {
        S->top++;
        S->array[S->top] = x;
    }
}

// Function to pop an element from the stack
int Pop(struct Stack *S) {
    if (StackEmpty(S)) {
        printf("Stack underflow\n");
        return -1; // You can choose to return a special value for underflow
    } else {
        int poppedElement = S->array[S->top];
        S->top--;
        return poppedElement;
    }
}

int main() {
    struct Stack myStack;
    InitializeStack(&myStack);

    // Push some elements onto the stack
    Push(&myStack, 10);
    Push(&myStack, 20);
    Push(&myStack, 30);

    // Check if the stack is empty
    if (StackEmpty(&myStack)) {
        printf("Stack is empty.\n");
    } else {
        printf("Stack is not empty.\n");
    }

    // Pop elements from the stack and print them
    printf("Popped elements: ");
    while (!StackEmpty(&myStack)) {
        int popped = Pop(&myStack);
        printf("%d ", popped);
    }
    printf("\n");

    return 0;
}

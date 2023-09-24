#include <stdio.h>
#include <stdbool.h>

#define MAX_SIZE 100 // Maximum capacity of the queue

// Queue structure
struct Queue {
    int array[MAX_SIZE];
    int front;
    int rear;
};

// Function to initialize an empty queue
void InitializeQueue(struct Queue *Q) {
    Q->front = Q->rear = -1; // Initialize front and rear to -1 to indicate an empty queue
}

// Function to check if the queue is empty
bool QueueEmpty(struct Queue *Q) {
    return Q->front == -1;
}

// Function to check if the queue is full
bool QueueFull(struct Queue *Q) {
    return (Q->rear == MAX_SIZE - 1) || (Q->rear == MAX_SIZE - 1 && Q->front == 0);
}

// Function to enqueue an element into the queue
void Enqueue(struct Queue *Q, int x) {
    if (QueueFull(Q)) {
        printf("Queue is full\n");
    } else {
        if (Q->front == -1) {
            Q->front = 0; // If the queue was empty, set front to 0
        }
        Q->rear++;
        Q->array[Q->rear] = x;
    }
}

// Function to dequeue an element from the queue
int Dequeue(struct Queue *Q) {
    if (QueueEmpty(Q)) {
        printf("Queue is empty\n");
        return -1; // You can choose to return a special value for underflow
    } else {
        int dequeuedElement = Q->array[Q->front];
        if (Q->front == Q->rear) {
            Q->front = Q->rear = -1; // If the queue becomes empty, reset front and rear
        } else {
            Q->front++;
        }
        return dequeuedElement;
    }
}

int main() {
    struct Queue myQueue;
    InitializeQueue(&myQueue);

    // Enqueue some elements into the queue
    Enqueue(&myQueue, 10);
    Enqueue(&myQueue, 20);
    Enqueue(&myQueue, 30);

    // Check if the queue is empty
    if (QueueEmpty(&myQueue)) {
        printf("Queue is empty.\n");
    } else {
        printf("Queue is not empty.\n");
    }

    // Dequeue elements from the queue and print them
    printf("Dequeued elements: ");
    while (!QueueEmpty(&myQueue)) {
        int dequeued = Dequeue(&myQueue);
        printf("%d ", dequeued);
    }
    printf("\n");

    return 0;
}

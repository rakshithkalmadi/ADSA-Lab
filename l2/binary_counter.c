#include <stdio.h>
#include <stdlib.h>

// Function to initialize a k-bit binary counter
void InitializeCounter(int *counter, int k) {
    for (int i = 0; i < k; i++) {
        counter[i] = 0;
    }
}

// Function to perform the INCREMENT operation on a k-bit binary counter
void IncrementCounter(int *counter, int k, int *amortizedCost) {
    int carry = 1; // Initialize carry to 1 for the increment
    int i = 0;

    while (carry && i < k) {
        if (counter[i] == 0) {
            counter[i] = 1;
            carry = 0; // No carry generated
        } else {
            counter[i] = 0;
            i++;
        }

        (*amortizedCost)++; // Increment the amortized cost for each bit checked
    }
}

int main() {
    int k = 4; // Change k as needed for different k-bit counters
    int counter[k];
    InitializeCounter(counter, k);
    int amortizedCost = 0;

    // Increment the counter 16 times and observe its state
    for (int i = 0; i < 16; i++) {
        IncrementCounter(counter, k, &amortizedCost);
        printf("Counter Value (Binary): ");
        for (int j = k - 1; j >= 0; j--) {
            printf("%d", counter[j]);
        }
        printf("\n");
    }

    printf("Amortized cost of increment operations: %d\n", amortizedCost);

    return 0;
}

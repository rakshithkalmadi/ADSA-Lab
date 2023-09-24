#include <stdio.h>

// Function to perform binary search
int binarySearch(int arr[], int left, int right, int target) {
    while (left <= right) {
        int mid = left + (right - left) / 2; // Calculate the middle index
        // If the target element is found at the middle
        if (arr[mid] == target) {
            return mid;
        }
        // If the target is in the left half of the array
        if (arr[mid] > target) {
            right = mid - 1;
        }
        // If the target is in the right half of the array
        else {
            left = mid + 1;
        }
    }
    // If the target is not in the array, return -1
    return -1;
}
int main() {
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int n = sizeof(arr) / sizeof(arr[0]);
    int target = 2;

    int result = binarySearch(arr, 0, n - 1, target);

    if (result != -1) {
        printf("Element found at index %d\n", result);
    } else {
        printf("Element not found in the array\n");
    }
    return 0;
}

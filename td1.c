#include<stdlib.h>
#include "hello.h"

void compare(const int a, const int b) {
    if (a < b) {
        printf("%d is less than %d\n", a, b);
    }
    else if (a > b) {
        printf("%d is greater than %d\n", a, b);
    }
    else {
        printf("%d is equal to %d\n", a, b);
    }
}

void assert(int assertion, int *variable){
    *variable = assertion;
    printf("Variable value: %d\n", *variable);
    printf("Variable address: %p\n", (void *)variable);
}

int sum(int a, int b) {
    return a + b;
}

int search(int *array, int size, int target) {
    for (int i = 0; i < size; i++) {
        if (array[i] == target) {
            return i; // Return the index of the target
        }
    }
    return -1; // Target not found
}

int binary_search(int *array, int size, int target) {
    int left = 0;
    int right = size - 1;

    while (left <= right) {
        int mid = (left + right) / 2;

        if (array[mid] == target) {
            return mid; 
        }

        else if (array[mid] < target) {
            left = mid + 1;
        }

        else {
            right = mid - 1;
        }
    }
    return -1; // Target not found
}

int main() {
    print_hello();
    compare(5, 10);

    for (int i = 1; i <= 100; i++) {
        printf("%d ", i);
    }

    int i = 1;
    while (i <= 100) {
        printf("%d ", i);
        i++;
    }

    assert(42, &i);

    int result = sum(5, 10);
    printf("Sum: %d\n", result);

    int array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int size = sizeof(array) / sizeof(array[0]);
    int target = 3;
    int index = search(array, size, target);
    if (index != -1) {
        printf("Target found at index: %d\n", index);
    } else {
        printf("Target not found\n");
    }

    int binary_index = binary_search(array, size, target);
    if (binary_index != -1) {
        printf("Target found at index: %d\n", binary_index);
    } else {
        printf("Target not found\n");
    }

    return 0;
}



#include<stdlib.h>
#include<stdio.h>
#include "hello.h"
#include<time.h>
#include<memory.h>
#include<unistd.h>
#include<semaphore.h>
#include<pthread.h>

sem_t semaphore;

void *print_message(void *ptr) {
    int id = *((int *)ptr);
    printf("\nThread %d essaie de se lancer\n", id);
    
    sem_wait(&semaphore); 
    
    printf("Début \n", id);
    sleep(2); 
    printf("Thread %d fini.\n", id);
    
    sem_post(&semaphore); 
    
    return NULL;
}

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

int binarySearch(int *array, int size, int target) {
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

int* selectionSort(int *array, int size) {

    int *array_copy = (int *)malloc(size * sizeof(int));
    for (int i = 0; i < size; i++) {
        array_copy[i] = array[i];
    }

    for (int j = 0; j < size - 1; j++) {
        
        int min = j;

        for (int i = j + 1; i < size; i++) {
            if (array_copy[i] < array_copy[min]) {
                min = i;
            }
        }

        int temp = array_copy[j];
        array_copy[j] = array_copy[min];
        array_copy[min] = temp;

    }

    return array_copy;
}

int main() {
    clock_t start, end;
    start = clock();


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

    // On lance le semaphore à 1 
    sem_init(&semaphore, 0, 1);

    pthread_t thread1, thread2, thread3;
    int id1 = 1, id2 = 2, id3 = 3;

    // On fait les threads
    if(pthread_create(&thread1, NULL, print_message, &id1)) {
        fprintf(stderr, "Erreur de creation thread 1\n");
        return 1;
    }
    if(pthread_create(&thread2, NULL, print_message, &id2)) {
        fprintf(stderr, "Erreur de création thread 2\n");
        return 1;
    }
    if(pthread_create(&thread3, NULL, print_message, &id3)) {
        fprintf(stderr, "Erreur de création thread 3\n");
        return 1;
    }

    // On attend que les threads soient finis
    if(pthread_join(thread1, NULL)) {
        fprintf(stderr, "Erreur join thread 1\n");
        return 2;
    }
    if(pthread_join(thread2, NULL)) {
        fprintf(stderr, "Erreur join thread 2\n");
        return 2;
    }
    if(pthread_join(thread3, NULL)) {
        fprintf(stderr, "Erreur join thread 3\n");
        return 2;
    }

    sem_destroy(&semaphore);

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

    

    int binary_index = binarySearch(array, size, target);
    if (binary_index != -1) {
        printf("Target found at index: %d\n", binary_index);
    } else {
        printf("Target not found\n");
    }

    int sorted_array[] = {64, 25, 12, 22, 11};
    int sorted_size = sizeof(sorted_array) / sizeof(sorted_array[0]);
    int *result_array = selectionSort(sorted_array, sorted_size);
    printf("Sorted array: ");
    for (int i = 0; i < sorted_size; i++) {
        printf("%d ", result_array[i]);
    }
    printf("\n");

    end = clock();
    double time_taken = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Execution time: %f seconds\n", time_taken);

    printf("On attend\n");

    sleep(10);

    printf("10 secondes");

    return 0;
}



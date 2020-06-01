#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <string.h>
#include <float.h>

pthread_mutex_t lock;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct node{
    int key;
    double val;
    struct node *next;
};

struct table{
    int size;
    struct node **list;
};

struct table *createTable(int size){
    struct table *t = malloc(sizeof(struct table));
    t->size = size;
    t->list = (struct node**)malloc(sizeof(struct node*)*size);
    int i;
    for(i=0;i<size;i++){
        t->list[i] = NULL;
        }
    return t;
}

int hashCode(struct table *t,int key){
    if(key<0)
        return -(key%t->size);
    return key%t->size;
}

void insert(struct table *t, int key, double val){
    int pos = hashCode(t,key);
    struct node *list = t->list[pos];
    struct node *newNode = malloc(sizeof(struct node));
    struct node *temp = list;
    while(temp){
        if(temp->key==key){
            temp->val = val;
            return;
        }
        printf("Here\n");
        temp = temp->next;
    }
    newNode->key = key;
    newNode->val = val;
    newNode->next = NULL;
    t->list[pos] = newNode;
}

double lookup(struct table *t, int key){
    int pos = hashCode(t,key);
    struct node *list = t->list[pos];
    struct node *temp = list;
    while(temp){
        if(temp->key==key){
            return temp->val;
        }
        temp = temp->next;
    }
    return -1;
}

//Sorting algorithms

// A function to implement bubble sort
void bubbleSort(int arr[], int n)
{
   int i, j;
   for (i = 0; i < n-1; i++){

       // Last i elements are already in place
       for (j = 0; j < n-i-1; j++){
           if (arr[j] > arr[j+1])
           {
            int temp = arr[j];
            arr[j] = arr[j+1];
            arr[j+1] = temp;
            }
        }
    }
}

// A function to sort arr using shellSort
int shellSort(int arr[], int n)
{
    // Start with a big gap, then reduce the gap
    for (int gap = n/2; gap > 0; gap /= 2)
    {
        for (int i = gap; i < n; i += 1)
        {
            // add a[i] to the elements that have been gap sorted
            // save a[i] in temp and make a hole at position i
            int temp = arr[i];

            // shift earlier gap-sorted elements up until the correct
            // location for a[i] is found
            int j;
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap)
                arr[j] = arr[j - gap];

            //  put temp (the original a[i]) in its correct location
            arr[j] = temp;
        }
    }
    return 0;
}

// A function to implement quicksort

void swap(int* a, int* b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

int partition (int arr[], int low, int high)
{
    int pivot = arr[high];    // pivot
    int i = (low - 1);  // Index of smaller element

    for (int j = low; j <= high- 1; j++)
    {
        // If current element is smaller than the pivot
        if (arr[j] < pivot)
        {
            i++;    // increment index of smaller element
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(int arr[], int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Function to sort an array using insertion sort
void insertionSort(int arr[], int n)
{
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

// Function to print an array
void printArray(int arr[], int size)
{
    int i;
    for (i=0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

void sort(int arr[], struct table *t){
    int n = 5;
    double totaltime;
    clock_t starttime, endtime;

    starttime = clock();
    bubbleSort(arr, n);
    endtime = clock();
    totaltime = ((double)(endtime - starttime)) / CLOCKS_PER_SEC;
    printf("Total time of bubble sort execution = %f \n", totaltime);
    printf("Sorted array: \n");
    printArray(arr, n);
    insert(t, 1, totaltime);

    starttime = clock();
    shellSort(arr, n);
    endtime = clock();
    totaltime = ((double)(endtime - starttime)) / CLOCKS_PER_SEC;
    printf("Total time of shell sort execution = %f \n", totaltime);
    printf("Sorted array: \n");
    printArray(arr, n);
    insert(t, 2, totaltime);

    starttime = clock();
    quickSort(arr, 0, n-1);
    endtime = clock();
    totaltime = ((double)(endtime - starttime)) / CLOCKS_PER_SEC;
    printf("Total time of quick sort execution = %f \n", totaltime);
    printf("Sorted array: \n");
    printArray(arr, n);
    insert(t, 3, totaltime);


    starttime = clock();
    insertionSort(arr, n);
    endtime = clock();
    totaltime = ((double)(endtime - starttime)) / CLOCKS_PER_SEC;
    printf("Total time of insertion sort execution = %f \n", totaltime);
    printf("Sorted array: \n");
    printArray(arr, n);
    insert(t, 4, totaltime);

}


int findSlowestAlgorithm(struct table *t){

    double maxValue = -1;
    int maxKey;
    for(int i= 0; i< t->size; i++){
    struct node *list = t->list[i];
    struct node *temp = list;
            if(temp->val > maxValue){
                maxValue = temp->val;
                maxKey = temp->key;
            }
            //temp = temp->next;
    }
    return maxKey;
}

int findFastestAlgorithm(struct table *t){

    double minValue = 20000;
    int minKey = 0;
    for(int i= 0; i< t->size; i++){
    struct node *list = t->list[i];
    struct node *temp = list;
    while(temp){
        if(temp->val < minValue){
            minValue = temp->val;
            minKey = temp->key;
        }
        temp = temp->next;
    }
   }
        return minKey;
}

void *findBestAndLastAlgorithm(void* arr){
    pthread_mutex_lock(&lock);
    printf("Lock thread \n");
    struct table *t = createTable(4);
    sort(arr, t);
    printf("Slowest algorithm: \n");
    printf("%d \n", findSlowestAlgorithm(t));
    printf("%f \n",lookup(t, findSlowestAlgorithm(t)));
    printf("Fastest algorithm: \n");
    printf("%d \n", findFastestAlgorithm(t));
    printf("%f \n",lookup(t, findFastestAlgorithm(t)));
    printf("Releasing lock \n \n \n");
    pthread_mutex_unlock(&lock);
    pthread_exit(NULL);

}

int main()
{
    int arr1[] = {12, 11, 13, 5, 6};
    int arr2[] = {13, 112, 1, 7, 80};
    int arr3[] = {2, 1, 67, 4, 65};
    int* arr[3];
    arr[0] = arr1;
    arr[1] = arr2;
    arr[2] = arr3;
    pthread_t* thread_id;
    thread_id = (pthread_t *) malloc(sizeof(arr1)*sizeof(pthread_t));
    int error;

    for (int i =0; i < 3; i++) {
        error = pthread_create(&thread_id[i], NULL, findBestAndLastAlgorithm, (void*)arr[i]);
        if (error != 0){
            printf("\nThread can't be created : [%s]", strerror(error));
        }
    }
      for (int j = 0; j < 3; ++j) {
        pthread_join(thread_id[j], NULL);
      }

    pthread_mutex_destroy(&lock);
    pthread_exit(NULL);
    return 0;
}

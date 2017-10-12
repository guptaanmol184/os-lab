#include <stdio.h>
#include <pthread.h>

int n=0;  // stores the size of the array
int arr[] = {10, 7, 8, 9, 1, 5, 20}; // array

// A utility function to swap two elements
void swap(int* a, int* b)
{
	int t = *a;
	*a = *b;
	*b = t;
}

/* This function takes last element as pivot, places
the pivot element at its correct position in sorted
array, and places all smaller (smaller than pivot)
to left of pivot and all greater elements to right
of pivot */
int partition (int arr[], int low, int high)
{
	int pivot = arr[high]; // pivot
	int i = (low - 1); // Index of smaller element

	for (int j = low; j <= high- 1; j++)
	{
		// If current element is smaller than or
		// equal to pivot
		if (arr[j] <= pivot)
		{
			i++; // increment index of smaller element
			swap(&arr[i], &arr[j]);
		}
	}
	swap(&arr[i + 1], &arr[high]);
	return (i + 1);
}

/* The main function that implements QuickSort
arr[] --> Array to be sorted,
low --> Starting index,
high --> Ending index */
void quickSort(int arr[], int low, int high)
{
	if (low < high)
	{
		/* pi is partitioning index, arr[p] is now
		at right place */
		int pi = partition(arr, low, high);

		// Separately sort elements before
		// partition and after partition
		quickSort(arr, low, pi - 1);
		quickSort(arr, pi + 1, high);
	}
}

void* qsrunner(void* params)
{

    int* ind = (int*) params;
    quickSort(arr, ind[0], ind[1]);

    pthread_exit(0);
}

/* Function to print an array */
void printArray(int arr[], int size)
{
	int i;
	for (i=0; i < size; i++)
		printf("%d ", arr[i]);
	printf("\n");
}

/* Merge two sub arrays */
void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
 
    int L[n1], R[n2];
 
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];
 
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
 
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }
 
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Driver program to test above functions
int main()
{
	n = sizeof(arr)/sizeof(arr[0]);

    // create threads
    pthread_t tid1, tid2;
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    int arr1[2] = {0, (n-1)/2};
    int arr2[2] = {((n-1)/2)+1, n-1};
    pthread_create(&tid1, &attr, &qsrunner, arr1);
    pthread_create(&tid2, &attr, &qsrunner, arr2);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    // Merge
    merge(arr, 0, (n-1)/2, n-1);

	/*quickSort(arr, 0, n-1);*/
	printf("Sorted array: \n");
	printArray(arr, n);
	return 0;
}


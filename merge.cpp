#include <iostream>
#include <chrono>
#include <iomanip>
#include <omp.h>

using namespace std;
using namespace std::chrono;

void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int L[n1], R[n2];

    for (int i = 0; i < n1; ++i)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];
}

void sequentialMergeSort(int arr[], int left, int right) {
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;

    sequentialMergeSort(arr, left, mid);
    sequentialMergeSort(arr, mid + 1, right);

    merge(arr, left, mid, right);
}

void parallelMergeSort(int arr[], int left, int right) {
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            parallelMergeSort(arr, left, mid);
        }

        #pragma omp section
        {
            parallelMergeSort(arr, mid + 1, right);
        }
    }

    merge(arr, left, mid, right);
}

int main() {
    int arr[] = {64, 25, 12, 22, 11};
    int n = sizeof(arr) / sizeof(arr[0]);

    cout << "Original array: ";
    for (int i = 0; i < n; ++i) {
        cout << arr[i] << " ";
    }
    cout << endl;

    // Sequential Merge Sort
    int arrSequential[n];
    for (int i = 0; i < n; ++i)
        arrSequential[i] = arr[i];

    auto startSeq = high_resolution_clock::now();
    sequentialMergeSort(arrSequential, 0, n - 1);
    auto stopSeq = high_resolution_clock::now();
    auto durationSeq = duration_cast<nanoseconds>(stopSeq - startSeq).count() / 1e9;
    cout << "Sequential Merge Sort Execution Time: " << fixed << setprecision(10) << durationSeq << " seconds" << endl;

    // Parallel Merge Sort
    int arrParallel[n];
    for (int i = 0; i < n; ++i)
        arrParallel[i] = arr[i];

    auto startPar = high_resolution_clock::now();
    parallelMergeSort(arrParallel, 0, n - 1);
    auto stopPar = high_resolution_clock::now();
    auto durationPar = duration_cast<nanoseconds>(stopPar - startPar).count() / 1e9;
    cout << "Parallel Merge Sort Execution Time: " << fixed << setprecision(10) << durationPar << " seconds" << endl;

    cout << "Sequential Sorted array: ";
    for (int i = 0; i < n; ++i) {
        cout << arrSequential[i] << " ";
    }
    cout << endl;

    cout << "Parallel Sorted array: ";
    for (int i = 0; i < n; ++i) {
        cout << arrParallel[i] << " ";
    }
    cout << endl;

    return 0;
}

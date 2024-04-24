#include <iostream>
#include <chrono>
#include <iomanip>
#include <omp.h>

using namespace std;
using namespace std::chrono;

// Sequential Bubble Sort
void sequentialBubbleSort(int arr[], int n) {
    bool swapped;
    auto start = high_resolution_clock::now();

    for (int i = 0; i < n - 1; ++i) {
        swapped = false;
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        // If no two elements were swapped in the inner loop, then the array is sorted
        if (!swapped)
            break;
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start).count() / 1e9;
    cout << "Sequential Bubble Sort Execution Time: " << fixed << setprecision(10) << duration << " seconds" << endl;
}

// Parallel Bubble Sort
void parallelBubbleSort(int arr[], int n) {
    bool swapped;
    auto start = high_resolution_clock::now();

    for (int i = 0; i < n - 1; ++i) {
        swapped = false;
        #pragma omp parallel for shared(arr, swapped)
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        // If no two elements were swapped in the inner loop, then the array is sorted
        if (!swapped)
            break;
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start).count() / 1e9;
    cout << "Parallel Bubble Sort Execution Time: " << fixed << setprecision(10) << duration << " seconds" << endl;
}

int main() {
    int arr[] = {64, 25, 12, 22, 11};
    int n = sizeof(arr) / sizeof(arr[0]);

    cout << "Original array: ";
    for (int i = 0; i < n; ++i) {
        cout << arr[i] << " ";
    }
    cout << endl;

    // Sequential Bubble Sort
    int arrSequential[n];
    copy(begin(arr), end(arr), arrSequential);
    sequentialBubbleSort(arrSequential, n);

    // Parallel Bubble Sort
    int arrParallel[n];
    copy(begin(arr), end(arr), arrParallel);
    parallelBubbleSort(arrParallel, n);

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

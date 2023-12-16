#include <iostream>
#include <fstream>
#include <vector>
#include <pthread.h>
#include <sys/time.h>

using namespace std;

// Structure to pass data to threads
struct ThreadData {
    vector<int>* array;
    int start;
    int end;
};

// Function to merge two sorted halves of an array
void merge(vector<int>& array, int start, int mid, int end) {
    int n1 = mid - start + 1;
    int n2 = end - mid;

    vector<int> left(n1);
    vector<int> right(n2);

    for (int i = 0; i < n1; i++)
        left[i] = array[start + i];
    for (int j = 0; j < n2; j++)
        right[j] = array[mid + 1 + j];

    int i = 0;
    int j = 0;
    int k = start;

    while (i < n1 && j < n2) {
        if (left[i] <= right[j]) {
            array[k] = left[i];
            i++;
        } else {
            array[k] = right[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        array[k] = left[i];
        i++;
        k++;
    }

    while (j < n2) {
        array[k] = right[j];
        j++;
        k++;
    }
}

// Function to perform merge sort for a range of elements
void* mergeSort(void* threadData) {
    ThreadData* data = (ThreadData*)threadData;

    if (data->start < data->end) {
        int mid = data->start + (data->end - data->start) / 2;

        // Create threads for the two halves
        pthread_t threadLeft, threadRight;

        ThreadData leftData{data->array, data->start, mid};
        ThreadData rightData{data->array, mid + 1, data->end};

        pthread_create(&threadLeft, NULL, mergeSort, (void*)&leftData);
        pthread_create(&threadRight, NULL, mergeSort, (void*)&rightData);

        // Wait for the two halves to finish sorting
        pthread_join(threadLeft, NULL);
        pthread_join(threadRight, NULL);

        // Merge the sorted halves
        merge(*data->array, data->start, mid, data->end);
    }

    pthread_exit(NULL);
}

// Function to read array from file
vector<int> readArrayFromFile(const string& fileName, int& numElements) {
    ifstream file(fileName);
    file >> numElements;

    vector<int> array(numElements);

    for (int i = 0; i < numElements; ++i) {
        file >> array[i];
    }

    return array;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <input_file>" << endl;
        return 1;
    }

    int numElements;
    vector<int> array = readArrayFromFile(argv[1], numElements);

    // Initialize pthread variables
    pthread_t mainThread;
    ThreadData mainData{&array, 0, numElements - 1};

    // Measure time before sorting
    timeval start, end;
    gettimeofday(&start, NULL);

    // Create the main thread for merge sort
    pthread_create(&mainThread, NULL, mergeSort, (void*)&mainData);

    // Wait for the main thread to finish sorting
    pthread_join(mainThread, NULL);

    // Measure time after sorting
    gettimeofday(&end, NULL);
    double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;

    // Output the sorted array
    for (int element : array) {
        cout << element << " ";
    }
    cout << endl;

    // Output the elapsed time
    cout << "END " << elapsed_time << endl;

    return 0;
}

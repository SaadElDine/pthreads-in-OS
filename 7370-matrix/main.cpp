#include <iostream>
#include <fstream>
#include <vector>
#include <pthread.h>
#include <sys/time.h>

using namespace std;

// Structure to pass data to threads
struct ThreadData {
    vector<vector<int>>* matrix1;
    vector<vector<int>>* matrix2;
    vector<vector<int>>* result;
    int row_start;
    int row_end;
};

// Function to perform matrix multiplication for a range of rows
void* multiplyRows(void* threadData) {
    ThreadData* data = (ThreadData*)threadData;

    for (int i = data->row_start; i < data->row_end; ++i) {
        for (int j = 0; j < data->matrix2->at(0).size(); ++j) {
            int sum = 0;
            for (int k = 0; k < data->matrix1->at(i).size(); ++k) {
                sum += data->matrix1->at(i)[k] * data->matrix2->at(k)[j];
            }
            data->result->at(i)[j] = sum;
        }
    }

    pthread_exit(NULL);
}

// Function to read matrix from file
vector<vector<int>> readMatrixFromFile(const string& fileName, int& rows, int& cols) {
    ifstream file(fileName);
    file >> rows >> cols;

    vector<vector<int>> matrix(rows, vector<int>(cols, 0));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            file >> matrix[i][j];
        }
    }

    return matrix;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <input_file>" << endl;
        return 1;
    }

    // Read matrices from file
    int rows1, cols1, rows2, cols2;
    vector<vector<int>> matrix1 = readMatrixFromFile(argv[1], rows1, cols1);
    vector<vector<int>> matrix2 = readMatrixFromFile(argv[1], rows2, cols2);

    // Check if matrices can be multiplied
    if (cols1 != rows2) {
        cerr << "Matrices cannot be multiplied. Invalid dimensions." << endl;
        return 1;
    }

    // Initialize result matrix
    vector<vector<int>> result(rows1, vector<int>(cols2, 0));

    // Measure time before multiplication
    timeval start, end;
    gettimeofday(&start, NULL);

    // Initialize pthread variables
    const int num_threads = 4;  // Adjust the number of threads as needed
    pthread_t threads[num_threads];
    ThreadData threadData[num_threads];

    // Divide work among threads
    int rows_per_thread = rows1 / num_threads;
    int remaining_rows = rows1 % num_threads;
    int current_row = 0;

    for (int i = 0; i < num_threads; ++i) {
        threadData[i].matrix1 = &matrix1;
        threadData[i].matrix2 = &matrix2;
        threadData[i].result = &result;
        threadData[i].row_start = current_row;
        threadData[i].row_end = current_row + rows_per_thread + (i < remaining_rows ? 1 : 0);

        pthread_create(&threads[i], NULL, multiplyRows, (void*)&threadData[i]);

        current_row = threadData[i].row_end;
    }

    // Wait for threads to finish
    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Measure time after multiplication
    gettimeofday(&end, NULL);
    double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;

    // Output the result matrix
    for (const auto& row : result) {
        for (int element : row) {
            cout << element << " ";
        }
        cout << endl;
    }

    // Output the elapsed time
    cout << "END1 " << elapsed_time << endl;

    return 0;
}

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
    int start;  // Start index for elements or rows
    int end;    // End index for elements or rows
};


void* multiplyElements(void* threadData) {
    // Cast the input data to the ThreadData structure
    ThreadData* data = (ThreadData*)threadData;

    // Loop through the specified range of elements (each elements) in a row for the result matrix
    for (int i = data->start; i < data->end; ++i) {
        // Calculate the row and column indices for the current element
        int row = i / data->result->at(0).size();
        int col = i % data->result->at(0).size();
        int sum = 0;

        // Loop through elements of the current row in the first matrix
        for (int k = 0; k < data->matrix1->at(row).size(); ++k) {
            // Perform the actual multiplication and summation
            sum += data->matrix1->at(row)[k] * data->matrix2->at(k)[col];
        }

        // Assign the calculated sum to the corresponding element in the result matrix
        data->result->at(row)[col] = sum;
    }

    // Exit the thread
    pthread_exit(NULL);
}


void* multiplyRow(void* threadData) {
    // Cast the input data to the ThreadData structure
    ThreadData* data = (ThreadData*)threadData;

    // Retrieve the row index for this thread
    int row = data->start;

    // Loop through columns of the result matrix
    for (int j = 0; j < data->matrix2->at(0).size(); ++j) {
        // Initialize a variable to store the sum for the current element in the result matrix
        int sum = 0;

        // Loop through elements of the current row in the first matrix
        for (int k = 0; k < data->matrix1->at(row).size(); ++k) {
            // Perform the actual multiplication and summation
            sum += data->matrix1->at(row)[k] * data->matrix2->at(k)[j];
        }

        // Assign the calculated sum to the corresponding element in the result matrix
        data->result->at(row)[j] = sum;
    }

    // Exit the thread
    pthread_exit(NULL);
}


// Function to read matrices from file
pair<vector<vector<int>>, vector<vector<int>>> readMatricesFromFile(const string& fileName, int& rows1, int& cols1, int& rows2, int& cols2) {
    ifstream file(fileName);
    file >> rows1 >> cols1;

    vector<vector<int>> matrix1(rows1, vector<int>(cols1, 0));

    for (int i = 0; i < rows1; ++i) {
        for (int j = 0; j < cols1; ++j) {
            file >> matrix1[i][j];
        }
    }

    file >> rows2 >> cols2;

    vector<vector<int>> matrix2(rows2, vector<int>(cols2, 0));

    for (int i = 0; i < rows2; ++i) {
        for (int j = 0; j < cols2; ++j) {
            file >> matrix2[i][j];
        }
    }

    // Return a pair of matrices
    return make_pair(matrix1, matrix2);
}



int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <input_file>" << endl;
        return 1;
    }

    // Read two matrices from file
    int rows1, cols1, rows2, cols2;
    pair<vector<vector<int>>, vector<vector<int>>> matrices = readMatricesFromFile(argv[1], rows1, cols1, rows2, cols2);

    vector<vector<int>>& matrix1 = matrices.first;
    vector<vector<int>>& matrix2 = matrices.second;

    // Check if matrices can be multiplied
    if (cols1 != rows2) {
        cerr << "Multipication of Matrix is not Possible !!" << endl;
        return 1;
    }

    // Initialize result matrix
    vector<vector<int>> result(rows1, vector<int>(cols2, 0));

    // Measure time before multiplication
    timeval start, end;
    gettimeofday(&start, NULL);

    // Initialize pthread variables for Variation 1
    const int num_threads_var1 = rows1 * cols2;  // One thread for each element
    pthread_t threads_var1[num_threads_var1];
    ThreadData threadData_var1[num_threads_var1];

    // Divide work among threads for Variation 1
    for (int i = 0; i < num_threads_var1; ++i) {
        threadData_var1[i].matrix1 = &matrix1;
        threadData_var1[i].matrix2 = &matrix2;
        threadData_var1[i].result = &result;
        threadData_var1[i].start = i;
        threadData_var1[i].end = i + 1;

        int th = pthread_create(&threads_var1[i], NULL, multiplyElements, (void*)&threadData_var1[i]);
        if (th) {
            cerr << "Error creating thread " << i << ". Code: " << th << endl;
            return 1;
        }
    }

    // Wait for threads to finish for Variation 1
    for (int i = 0; i < num_threads_var1; ++i) {
        pthread_join(threads_var1[i], NULL);
    }

    // Measure time after Variation 1
    gettimeofday(&end, NULL);
    double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;

    // Output the result matrix for Variation 1
    for (const auto& row : result) {
        for (int element : row) {
            cout << element << " ";
        }
        cout << endl;
    }


    // Output the elapsed time for Variation 1
    cout << "END1 " << elapsed_time << endl;


    // Initialize pthread variables for Variation 2
    const int num_threads_var2 = rows1;  // One thread for each row
    pthread_t threads_var2[num_threads_var2];
    ThreadData threadData_var2[num_threads_var2];

    // Measure time before multiplication
    gettimeofday(&start, NULL);

    // Divide work among threads for Variation 2
    for (int i = 0; i < num_threads_var2; ++i) {
        threadData_var2[i].matrix1 = &matrix1;
        threadData_var2[i].matrix2 = &matrix2;
        threadData_var2[i].result = &result;
        threadData_var2[i].start = i;

        pthread_create(&threads_var2[i], NULL, multiplyRow, (void*)&threadData_var2[i]);
    }

    // Wait for threads to finish for Variation 2
    for (int i = 0; i < num_threads_var2; ++i) {
        pthread_join(threads_var2[i], NULL);
    }

    // Measure time after Variation 2
    gettimeofday(&end, NULL);
    elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;

    // Output the result matrix for Variation 2
    for (const auto& row : result) {
        for (int element : row) {
            cout << element << " ";
        }
        cout << endl;
    }

    // Output the elapsed time for Variation 2
    cout << "END2 " << elapsed_time << endl;

    return 0;
}
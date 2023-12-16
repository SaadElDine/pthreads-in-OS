# Multi-threaded Matrix Multiplication
- This C++ program demonstrates multi-threaded matrix multiplication using pthreads. The implementation includes two variations:

1. Element-wise Computation in Threads
Each element of the output matrix is computed concurrently in a separate thread.
2. Row-wise Computation in Threads
Each row of the output matrix is computed concurrently in a separate thread.
Usage
Compile the program using:

bash

g++ matrix_multiplication.cpp -o matrix_multiplication -lpthread
Run the program:

bash
./matrix_multiplication input.txt
Input Format
The program reads two input matrices from a file in the following format:

css

[number of rows of 1st matrix] [number of columns of 1st matrix]
1st matrix entries
[number of rows of 2nd matrix] [number of columns of 2nd matrix]
2nd matrix entries
Example Input
diff
3 5
1 -2 3 4 5
1 2 -3 4 5
-1 2 3 4 5
5 4
-1 2 3 4
1 -2 3 4
1 2 -3 4
1 2 3 -4
-1 -2 -3 -4
Output Format
The program outputs the result matrix entries and the elapsed time for each variation:

sql
[result matrix entries]
END1 [elapsed time of procedure 1]
[result matrix entries]
END2 [elapsed time of procedure 2]
Multi-threaded Merge Sort
This C++ program demonstrates multi-threaded merge sort using pthreads.

Usage
Compile the program using:

bash
g++ merge_sort.cpp -o merge_sort -lpthread
Run the program:

bash
./merge_sort input.txt

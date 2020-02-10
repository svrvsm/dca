# dca
Environment:
Ubuntu 16.04 - 64 bit
GCC version - gcc (Ubuntu 6.5.0-2ubuntu1~16.04) 6.5.0 20181026

Application output - dca.
Design and assumptions are in the respective headers.

Steps to build
1. Extract to directory.
2. cd to that top level directory "dca"
3. $make all

Running the application:
4. $./dca
5. When prompted for "Enter file name or < Ctrl+C > to exit", enter the full absolute path of the input file.
6. prints error or success.
5. $./output_for_<input_filename> gets generated.
6.  "log.txt" (with debug info) gets generated in the current directory 



# Computing-Eigenvalues-and-Eigenvectors-using-Normalized-Power-Iteration-together-with-Deflation
This C++ program is designed for finding most dominant eigenvalue and related eigenvector and  next dominant eigenvalue of a square matrix using normalized power iteration with deflation.

In order to run the program:

1)You need to compile the source code file (UtkuTurkbey.cpp) with a compiler, and create 
"UtkuTurkbey.exe" executable file.

2)You need a text file of matrix data named arbitrarily and to be stored in the same 
folder with the "UtkuTurkbey.exe" file. This text file must include nxn matrix elements written in
the order of "a11 a12 ... a1n a21 .... a2n .... an1 .... ann"(order is strict, 
giving only one row in one line is preferred)
CAUTION!!! In order to have correct results, input data(Matrix A) must obey these preconditions
(No additional errors will be printed if such conditions violeted): 
a) Matrix A must be square(nxn).
b) Matrix A must be non-singular.
c) First and next dominant eigenvalue of matrix A must have algebraic multiplicity of 1.
d) Matrix A must have at least two distinct real eigenvalues.
e) All elements of matrix A must be real.
f) If matrix A is 1x1 then the first eigenvalue will be itself and related eigenvector will be 1 but second eigenvalue will be 
trivial eigenvalue zero which is true for all matrices but useless.

3)You need to run a command line, direct the location to the folder where "UtkuTurkbey.exe" file is located
and type: <UtkuTurkbey.exe Input.txt 1e-6 Output.txt> without <> parts. In this example "Input" is the name of the matrix data text file;
"1e-6" is the precision value to be used in power iteration and "Output" is the name of the text file output of the program 
will be written. Please change these names accordingly wrt your input file name, desired precision and desired output file name. 
Then press the enter.


Output of the program:

Value of the most dominant eigenvalue of the matrix is shown as "Eigenvalue #1".
Related eigenvector for the most dominant eigenvalue is printed, one element per line, in total "n" line.
Next most dominant eignvalue of the matrix is shown as "Eigenvalue #2".

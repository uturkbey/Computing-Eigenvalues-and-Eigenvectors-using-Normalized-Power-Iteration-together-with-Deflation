#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>

using namespace std;

//THIS CLASS CALLED MATRIX, REPRESENTS A 2D MATRIX AND IMPLEMENTS BASIC MATRIX OPERATIONS  
class Matrix
{
  public:
	//instance variables
	int row; //number of rows in matrix
	int column; //number of columns in matrix
	double ** matrix; //2D double array holding (row x column) matrix data
	
	//constructer
	Matrix (int, int); 
		
	//instance methods
	void set (int, int, double);
	void set (Matrix&);
	double get (int, int);
	Matrix multiplication (Matrix&);
	Matrix multiplication (double);
	Matrix subtraction (Matrix&);
	Matrix transpose (void);
	double getInfNorm (void);
	Matrix normalize(void);
	double getLargestByMag(void);
};

//This constructor creates an empty matrix of size rxc
Matrix::Matrix(int r, int c)
{
	row = r;
	column = c;	
	
	matrix = new double * [r];
    for(int i = 0; i < r; i++)
    {
    	matrix[i] = new double [c];
	}
}

//This method returns a specific element specified by arguments
double Matrix::get(int r, int c)
{
	return matrix[r-1][c-1];
}

//This method sets a specific element of matrix to value given as argument
//Assumed that no erroneous index is given
void Matrix::set(int r, int c, double value)
{
	matrix[r-1][c-1] = value;
}

//This methods sets given matrix as paramter to this matrix
void Matrix::set(Matrix& other)
{
	this->row = other.row;
	this->column = other.column;
	this->matrix = other.matrix;
}

//This method multiplies this matrix and the one passed as argument, then returns the solution matrix
//Current matrix and supplied matrix remains unchanged, it is assumed that this.column and other.row are equal
Matrix Matrix::multiplication(Matrix& other)
{
	Matrix result (this->row, other.column); //create an empty matrix of size this.row x m.column
	double sum = 0; //a dummy variable to hold result of each row and column multiplication. 
	for (int i = 1; i <= result.row; i++) //current row for result
	{
		for (int j = 1; j <= result.column; j++) //current column for result
		{
			sum = 0;
			for (int m = 1; m <= this->column; m++)//current column for this, current row for other
			{
				sum = sum + this->get(i, m) * other.get(m, j);
			}
			result.set(i, j, sum);
		}
	}
	return result;
}

//This method multiplies this matrix by a sclar given as argument, then returns the solution matrix
//Current matrix remains unchanged
Matrix Matrix::multiplication(double scalar)
{
	Matrix result (this->row, this->column);
	for (int i = 1; i <= result.row; i++) //current row for result
	{
		for (int j = 1; j <= result.column; j++) //current column for result
		{
			result.set(i, j, this->get(i, j) * scalar);
		}
	}
	return result;
}

//This method substracts the the matrix given as argument from this matrix and returns the solution
//This and other matrices remain unchanged, it is assumed that dimensions of this and other are same
Matrix Matrix::subtraction(Matrix& other)
{
	Matrix result (this->row, this->column);
	for (int i = 1; i <= result.row; i++) //current row for result
	{
		for (int j = 1; j <= result.column; j++) //current column for result
		{
			result.set(i, j, this->get(i, j) - other.get(i, j));
		}
	}
	return result;
}

//This method returns the transpose of this matrix
//This matrix remains unchanged
Matrix Matrix::transpose()
{
	Matrix result (this->column, this->row);
		for (int i = 1; i <= result.row; i++) //current row for result
	{
		for (int j = 1; j <= result.column; j++) //current column for result
		{
			result.set(i, j, this->get(j, i));
		}
	}
	return result;
} 

//This method works for only nx1 matrices, i.e. vectors, and returns infinity norm of the vector. 
//Otherwise it will result in wrong solution and will only return infinity norm of first column vector of the matrix 
double Matrix::getInfNorm()
{
	double max = abs(this->get(1,1));
	for (int i = 2; i <= this->row; i++)
	{
		if (abs(this->get(i,1)) > max)
		{
			max = abs(this->get(i,1));
		}
	}
	return max;
}

//This method works for only nx1 matrices, i.e. vectors, and returns the normalized version of the vector.
//Original vector remains unchanged
Matrix Matrix::normalize()
{
	Matrix result (this->row, 1);
	double largestElement = this->getLargestByMag();
	for (int i = 1; i <= this->row; i++)
	{
		result.set(i, 1, this->get(i, 1) / largestElement);
	}
	return result;
}

//This method works for only nx1 matrices, i.e. vectors, and returns element with largest magnitude. 
//Otherwise it will result in wrong solution and will only return element with largest magnitude of first column vector of the matrix 
double Matrix::getLargestByMag()
{
	int indexOfLargest = 1;
	double max = abs(this->get(1,1));
	for (int i = 2; i <= this->row; i++)
	{
		if (abs(this->get(i,1)) > max)
		{
			max = abs(this->get(i,1));
			indexOfLargest = i;
		}
	}
	return this->get(indexOfLargest, 1);
}

//MAIN FUNCTION FOR NORMALIZED POWER ITERATION TOGETHER WITH DEFLATION TO COMPUTE EIGENVALUES AND EIGENVECTORS
int main (int argc, char * argv[]) //In order to give command line command necessary arguments added to the main function
{
	
	//1)DECLARATION OF NECESSARY VARIABLES TO BE USED LATER
	
	ifstream myInputFile; //stream object in order to read from files
	ofstream myOutputFile; //stream object in order to write to files
	
	const double precision = atof(argv[2]); //Precision given by the user. "atof" function functionality is learned from https://stackoverflow.com/questions/13424265/convert-a-char-to-double/13424364  
	
	int size = 0; //to make it has a meaningfull name, it is called size, size of one dimension of our square matrix A.
				  //simply it can be called also n for nxn matrix A 
				  //it will be first set equal to n^2 but then square root will be calculated and set to size variable

	double currentInput; //a dummy variable to hold the read data, during measuring size of the matrix A
						 

	//2)READING INPUT MATRIX A FROM TEXT FILE GIVEN AS COMMAND LINE ARGUMENTS USING DYNAMIC MEMORY ALLOCATION
 	
	/*2.1)We have to determine the size of the matrix A in order to allocate dynamic memory.  
	Therefore we go through the A matrix data and count the number of elements. Since the number of elements
	within this matrix is also equal to the square of number of elements in each row of square matrix A, we will
	derive the number "n"(called size variable) by taking the square root of the size variable and set it equal to itself.    	
	*/
	myInputFile.open(argv[1]); //open matrix A data file
    while ( myInputFile >> currentInput) //as long as an input exists keep on scanning, increment size in each step 
    {
    	size = size + 1; 
	}
	myInputFile.close(); //close matrix A data file
	size = sqrt(size); //initially size was equal to n^2 but now it is set equal to n which is the dimension of matrix.
	   
    //2.2)Since we now know the size of matrix A we can initialize A and read it from the file 
	Matrix a (size, size);	//Matrix object for holding matrix A
	myInputFile.open(argv[1]); //open the matrix data file 
	for (int i = 1; i <= size; i++) //travel through rows
	{
		for (int j = 1; j <= size; j++) //travel through columns of each row
		{
			myInputFile >> currentInput;
			a.set(i, j, currentInput);
		}
	}
	myInputFile.close();//close matrix data file 
	
	//3)COMPUTE DOMINANT EIGENVALUE AND RELATED EIGENVECTOR OF MATRIX A USING NORMALIZED POWER ITERATION (Micheal T. Heath, Scientific Computing, 2nd ed., p.175, Algorithm 4.2)
	
	//3.1)We need a random vector to be our x0 vector for Power iteration. size x 1 vector full of 1s will be used for simplicity.
	Matrix xk (size, 1); //empty xk vector
	for (int i = 1; i <= size; i++) //set x0 to 1s vector to be used in 3.2 
	{
		xk.set(i, 1, 1);
	}
	Matrix yk (size, 1); //empty yk vector to be used in 3.2 
	
	//3.2)We apply power iteration (Micheal T. Heath, Scientific Computing, 2nd ed., p.175, Algorithm 4.2)
	double currentEigenValue; //to hold current approximate eigenvalue
	do
	{
		yk = a.multiplication(xk); //generate next vector
		currentEigenValue = yk.getLargestByMag(); //record current eigenvalue which is the infinity norm of new vector yk 
		xk = yk.normalize(); //normalize the current vector
	} while ( ( (a.multiplication(xk)).subtraction(yk) ).getInfNorm() > precision  );
	
	 
	//3.3)Print current eigenvalue and eigenvector to output file because it will be lost in the following parts since xk and currentEigenvalue will be used again in part 4 and 5
	myOutputFile.open(argv[3]); //Name of the text file is given as command line argument
	//These two lines are for fixing number of digits in fractional part after decimal point. Learned from https://stackoverflow.com/questions/5907031/printing-the-correct-number-of-decimal-points-with-cout
	myOutputFile << fixed; 
	myOutputFile << setprecision(2);
	
	myOutputFile << "Eigenvalue#1: " << currentEigenValue << "\n";
	for (int i = 1; i <= size; i++)
	{
		myOutputFile << xk.get(i, 1) << "\n";
	}
	
	//4)APPLY DIFLATION TO MATRIX A IN ORDER TO FIND NEXT DOMINANT EIGENVALUE (Micheal T. Heath, Scientific Computing, 2nd ed., p.179-180, Alternative Approach)
	
	//4.1) Find ek vector, kth standart unit vector 
	//xk now is our normalized first eigenvector related to dominant eigenvalue. At least one of the elements is equal to 1. 
	//An problem due to finite precision artihmethic when using "== 1" is avoided because Matrix::normalize() ensures there will be at least one "1".
	int k = 1; //a variable to keep place of "1" in xk vector
	for (int i = 2; i <= size; i++) //find poisiton of "1"
	{
		if ( xk.get(i,1) == 1)
		{
			k = i;
		}
	}
	Matrix ek (size, 1); // empty vector to be kth standart unit vector
	for (int i = 1; i <= size; i++)
	{
		if (i == k)
		{
			ek.set(i, 1, 1); // "1" in position k
		}
		else
		{
			ek.set(i, 1, 0); //"0" elsewhere
		}
	}
	
	//4.2) Find u1 vector
	Matrix u1 = (a.transpose()).multiplication(ek); //Multiply transpose of A with ek
	 
	//4.3)Modify A matrix such that previously found dominant eigenvalue is removed
	u1 = u1.transpose(); //Set u1 to u1^T
	xk = xk.multiplication(u1);  //Set xk to xk * u1^T
	a = a.subtraction(xk); //Set A to A - xk * u1^T
	//	a = a.subtraction( xk.multiplication( u1.transpose() ) ); //THIS CODE DOES NOT COMPÝLE THEREFORE THREE STEPS ABOVE USED 
	
	//5)APPLY NORMALIZED POWER ITERATION TO DIFLATED MATRIX AND FIND NEXT DOMINANT EIGENVALUE
	//This part is copied from part 3 and is identical
	
	//5.1) Choose a random x0 vector
	for (int i = 1; i <= size; i++) //set xk to x0 = 1s vector to be used in 5.2 
	{
		xk.set(i, 1, 1);
	}
	
	//5.2)We apply power iteration (Micheal T. Heath, Scientific Computing, 2nd ed., p.175, Algorithm 4.2)
	do
	{
		yk = a.multiplication(xk); //generate next vector
		currentEigenValue = yk.getLargestByMag(); //record current eigenvalue which is the infinity norm of new vector yk 
		xk = yk.normalize(); //normalize the current vector
	} while ( ( (a.multiplication(xk)).subtraction(yk) ).getInfNorm() > precision  );
	
	//5.3)Print current eigenvalue to output file
	 myOutputFile << "Eigenvalue#2: " << currentEigenValue << "\n";
	
	//Close used files
	myInputFile.close();
	myOutputFile.close();
    
    return 0;
}

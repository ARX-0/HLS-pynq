//The example shows the correct coding guidlenes for nested loops to be flattened automatically by the tool
#include "loop_imperfect.hpp"

void loop_perfect(din_t A[N], dout_t B[N]) {
    int i, j;
    dint_t acc;

LOOP_I:
    for (i = 0; i < 20; i++) { // if this is pipelined
    LOOP_J:                        // |
        for (j = 0; j < 20; j++) { // \/ this will get unrolled

            if (j == 0)
                acc = 0;
            acc += A[j] * j;
            if (j == 19) {
                if (i % 2 == 0)
                    B[i] = acc / 20;
                else
                    B[i] = 0;
            }
        }
    }
}


/*
// ---------------- Inverse function ---------------- //


bool qbMatrix2<T>::Inverse() {
    if(!IsSquare()) {
        throw std::invalid_argument("Matrix must be square to compute inverse");
    }

    //if we get to here the matrix is square, and invertable.

    //form an identity matrix of the same size as this matrix that you wish to invert
    qbMatrix2<T> identityMatrix(m_nRows, m_nCols);
    identityMatrix.SettoIdentity();

int originalNumCols = m_nCols;
    //Join the identity matrix to this matrix to form an augmented matrix
    Join(identityMatrix);
    
    //Begin the Gauss Jordan elimination process
    int cRow, cCol;
    int maxCount = 100;
    int count = 0;
    bool completeFlag = false;
    while((!completeFlag) && (count < maxCount)){
        for (int diagIndex=0; diagIndex <m_nRows ; ++diagIndex){
            cRow = diagIndex;
            cCol = diagIndex;

            //find the row with the largest element in this column
            int maxIndex = FindRowWithMaxElement(cCol, cRow);

            if(maxIndex != cRow){
                //swap the current row with the row with the largest element
                //std::cout << "Swapping row " << cRow << " with row " << maxIndex << std::endl;
                SwapRow(cRow, maxIndex);
            }
            
            //make the diagonal element 1 by multiplying the row by the inverse of the diagonal element
            if(m_matrix_Data[Sub2Ind(cRow,cCol)] != 0){
                T multFactor = 1.0 / m_matrix_Data[Sub2Ind(cRow,cCol)];
                RowMult(cRow, multFactor);
                //std::cout << "Making diagonal element 1 by multiplying row " << cRow << " by " << multFactor << std::endl;
            }

//Consider the Columns
for(int rowIndex = cRow+1 ; rowIndex<m_nRows ; ++rowIndex){

if(!CloseEnough(m_matrix_Data[Sub2Ind(rowIndex,cCol)],0.0)){
//Obtain the element to work with from the matrix diagonal
//As we aim to set all the diagonal elements to 1, this should
//always be valid for a matrix that can be inverted
int rowOneIndex = cCol;

//Get the value stored at the current element.
T currentElement = m_matrix_Data[Sub2Ind(rowIndex,cCol)];
// value stored as (rowIndex, cCol) in the matrix
T rowOneValue = m_matrix_Data[Sub2Ind(rowOneIndex,cCol)];

if(!CloseEnough(rowOneValue,0.0)){
//Calculate the multiplication factor to be used to eliminate the current element
T correctionFactor = -(currentElement/rowOneValue);

MultAdd(rowIndex, rowOneIndex, correctionFactor);

//std::cout << "Multiply row " << rowOneIndex << " by " << correctionFactor << " and add to row " << rowIndex << std::endl;
// "and add to row " << rowIndex << std::endl;
}
}
//std::cout << "Eliminating element at (" << rowIndex << "," << c
}


//consider the row.
for(int colIndex = cCol+1 ; colIndex<m_nCols ; ++colIndex){
    if(!CloseEnough(m_matrix_Data[Sub2Ind(cRow,colIndex)],0.0)){
        //Obtain the element to work with from the matrix diagonal
        //As we aim to set all the diagonal elements to 1, this should
        //always be valid for a matrix that can be inverted.
        int rowOneIndex = cCol;
        //Get the value stored at the current element.
        T currentElement = m_matrix_Data[Sub2Ind(cRow,colIndex)];
        //get the value stored at the diagonal element
        T rowOneValue = m_matrix_Data[Sub2Ind(rowOneIndex,colIndex)];
        //if this is equal to zero, we cannot proceed
        if(!CloseEnough(rowOneValue,0.0)){
            //compute the correction factor
            // (required to reduce the element at (cRow,colIndex) to zero.
            T correctionFactor = -(currentElement/rowOneValue);
            //To make the element at (cRow,colIndex) zero, we need to add -currentElement multiplied by the row containing the diagonal element
            //the row at rowOneIndex
            MultAdd(cRow, rowOneIndex, correctionFactor);
        }
    }
}

//separate the result into the left and the right halves 

qbMatrix2<T> leftHalf;
qbMatrix2<T> rightHalf;
this->Seperate(&leftHalf, &rightHalf, originalNumCols);

//When the process is complete, the left half should be the identity matrix, and the right half should be the inverse of the original matrix
if(leftHalf == identityMatrix){
    //set completedFlag to true to indicate success
    completeFlag = true;

    //rebuild this matrix to be the right half
    m_nCols = originalNumCols;
    m_nElements = m_nRows * m_nCols;
    delete[] m_matrix_Data;
    m_matrix_Data = new T[m_nElements];
    for(int i=0 ; i<m_nElements ; ++i){
        m_matrix_Data[i] = rightHalf.m_matrix_Data[i];
    }
    //increment the count
    count++;
}
//return if the process was successful
 return completeFlag;
    }
}
}
///////////////////////////////////////////////////////////////////////////////



/*
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <math.h>
#include <sstream>
#include <fstream>

#include "matrix_manip_cleanup.h"

using namespace std;

// ---------------- Print ---------------- //
/*
template<class T>
void qbMatrix2<T>::PrintMatrix() {
    for (int i = 0; i < m_nRows; i++) {
        for (int j = 0; j < m_nCols; j++)
            std::cout << std::setw(10) << m_matrix_Data[Sub2Ind(i, j)] << " ";
        std::cout << "\n";
    }
}
*/
int main() {
    std::string filename = "test_matrices.csv";

    /*Openfile*/
    ifstream input(filename);
    if(input.is_open()){

        //the file has one header line containing the number of rows and columns of each matrix
        std::string currline;
        getline(input, currline);

        // Accept both "# n" and "n" as header
        int nRows = 0;
        size_t spacepos = currline.find(" ");
        if (spacepos != std::string::npos) {
            std::string nrows_str = currline.substr(spacepos, currline.length());
            nRows = atoi(nrows_str.c_str());
        } else {
            nRows = atoi(currline.c_str());
        }

//Read the rest of the file and parse each line as we go.
std::stringstream ss;
double currentNumber;

/*currentRow will hold all the elements that make up the row we have just read, which will correspond to a single line form the .CSV file*/
std::vector<double> currentRow;
/*currentRow will hold all the data from the elements that make up the row we have just read,
which will correspond to the single line from the CSV file. */

std::vector<double> currentMatrixData;

/*Create a vector of qbMatrix2 pointers so that we acan hold the matrices that resuly in any failed tests */
std::vector<qbMatrix2<double>*> failedMatrices;

int totalRows = 0;
int rowCount = 0;
int numSuccess = 0;
int numFailures = 0;
//loop untill we reach the EOF

while (!input.eof()){
    getline(input, currline);
    
    //convert to string stream object   
    ss<< currline;
    //loop through the string stream to extract the numbers around the commas
    while(ss >> currentNumber){
        currentRow.push_back(currentNumber);
        if(ss.peek() == ','){
            ss.ignore();
        }
    }

    //Clear the string stram ready for the next loop, since we don't create a new one each time
    // (currentRow now contains all the numbers we need)
    ss.clear();

    /*concatinate the elsments of the current row into the currentmatrix, which will be one large and single dimentional array*/
    currentMatrixData.insert(currentMatrixData.end(), currentRow.begin(), currentRow.end());

    //Clear the current row ready for the next loop
    currentRow.clear();

    //Incerement the row count
    rowCount++;

    /*if we have read 5 rows, then w stop and form this data into a qbMatrix2 object with dimentions of 5 rows and 10 cols*/
    if(rowCount > 4){
        //Reset the row count
        rowCount = 0;
        //create the qbMatrix2 object object and store the matrix data
        qbMatrix2<double> newMatrix(5,10,&currentMatrixData);

        //prepare the two matrices to contatin the left and the right halves of this matrix
        // (each of these will be square, with dimentions of 5x5)
        qbMatrix2<double> leftMatrix;
        qbMatrix2<double> rightMatrix;
        //seperate the new matrix into its left and right halves
        newMatrix.Seperate(&leftMatrix, &rightMatrix, 5);

        /*the right hand side now contains the golden result (inverse) loaded from the CSV file, while the left hand side is original matrix, Therefore we want to invert the 
        left hand side ourselfs and then compare the result with teh right hand side*/

        leftMatrix.Inverse();

        /*note the use of the 'compare' function with a specified tolerance of 1e-9.*/
        if(leftMatrix.Compare(rightMatrix, 1e-9)){
            numSuccess++;
            //std::cout << "Test " << totalRows/5 << " passed" << std::endl;
            std::cout << "Test passed" << std::endl;
        }
        else{
            numFailures++;
            //std::cout << "Test " << totalRows/5 << " failed" << std::endl;
            std::cout << "Test failed" << std::endl;
            //store a pointer to the failed matrix
            failedMatrices.push_back(new qbMatrix2<double>(newMatrix));
        }

        currentMatrixData.clear();

    }
    totalRows++;

}

cout<<"Num success = " << numSuccess << std::endl;
cout<<"Num failures = " << numFailures << std::endl;

//print out any failed matrices
for(qbMatrix2<double>* currentMatrix : failedMatrices){
    currentMatrix->PrintMatrix();
}

input.close();
    return 0;
}
}
*/

*/



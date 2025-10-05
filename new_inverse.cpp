#ifndef QBMATRIX2_H
#define QBMATRIX2_H

#include <vector>
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <iomanip>

// Template class qbMatrix2 for generic matrix handling
template <class T>

template<class T>
bool qbMatrix2<T>::resize(int numRows, int numCols) {
    if (numRows < 1 || numCols < 1) return false;
    delete[] m_matrix_Data;
    m_nRows = numRows;
    m_nCols = numCols;
    m_nElements = m_nRows * m_nCols;
    m_matrix_Data = new T[m_nElements];
    for (int i = 0; i < m_nElements; i++)
        m_matrix_Data[i] = 0.0;
    return true;
}

template<class T>
void qbMatrix2<T>::SettoIdentity() {
    if (!IsSquare()) throw std::runtime_error("Identity only for square matrices");
    for (int i = 0; i < m_nRows; i++)
        for (int j = 0; j < m_nCols; j++)
            m_matrix_Data[Sub2Ind(i, j)] = (i == j) ? 1.0 : 0.0;
}

// ---------------- Element Access ---------------- //

template<class T>
T qbMatrix2<T>::GetElement(int row, int col) const {
    if (row < 0 || row >= m_nRows || col < 0 || col >= m_nCols)
        throw std::out_of_range("Index out of range");
    return m_matrix_Data[Sub2Ind(row, col)];
}

template<class T>
bool qbMatrix2<T>::SetElement(int row, int col, const T& value) {
    if (row < 0 || row >= m_nRows || col < 0 || col >= m_nCols)
        return false;
    m_matrix_Data[Sub2Ind(row, col)] = value;
    return true;
}

template<class T>
int qbMatrix2<T>::GetNumRows() const { return m_nRows; }

template<class T>
int qbMatrix2<T>::GetNumCols() const { return m_nCols; }

// ---------------- Utility Functions ---------------- //

template<class T>
int qbMatrix2<T>::Sub2Ind(int row, int col) const {
    return row * m_nCols + col;
}

template<class T>
bool qbMatrix2<T>::IsSquare() { return m_nRows == m_nCols; }

template<class T>
bool qbMatrix2<T>::CloseEnough(T f1, T f2) {
    return std::fabs(f1 - f2) < 1e-9;
}

// ---------------- Print ---------------- //

template<class T>
void qbMatrix2<T>::PrintMatrix() {
    for (int i = 0; i < m_nRows; i++) {
        for (int j = 0; j < m_nCols; j++)
            std::cout << std::setw(10) << m_matrix_Data[Sub2Ind(i, j)] << " ";
        std::cout << "\n";
    }
}

#endif

template <class T>
bool qbMatrix2<T>::Inverse() {
    IF(!IsSquare()) {
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
                T multFactor = 1.0 / m_matrixData[Sub2Ind(cRow,cCol)];
                RowMult(cRow, multFactor);
                //std::cout << "Making diagonal element 1 by multiplying row " << cRow << " by " << multFactor << std::endl;
            }

//Consider the Columns
for(int rowIndex = cRow+1 ; rowIndex<m_nRows ; ++rowIndex){
if(!CloseEnough(m_matrixData[Sub2Ind(rowIndex,cCol)],0.0)){
//Obtain the element to work with from the matrix diagonal
//As we aim to set all the diagonal elements to 1, this should
//always be valid for a matrix that can be inverted
int rowOneIndex = cCol;

//Get the value stored at the current element.
T currentElement = m_matrixData[Sub2Ind(rowIndex,cCol)];
// value stored as (rowIndex, cCol) in the matrix
T rowOneValue = m_matrixData[Sub2Ind(rowOneIndex,cCol)];

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
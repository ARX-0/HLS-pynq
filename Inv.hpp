#ifndef QBMATRIX2_H
#define QBMATRIX2_H

#include <vector>
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <iomanip>

// Template class qbMatrix2 for generic matrix handling
// Provides constructors, destructors, element access, resize, identity, comparison,
// operator overloading, and utility functions for linear algebra-like operations.

template <class T>
class qbMatrix2
{
public:
    // Constructors
    qbMatrix2();
    qbMatrix2(int nRow, int nCol);
    qbMatrix2(int nRow, int nCol, const T* inputdata);
    qbMatrix2(const qbMatrix2<T>& inputMatrix);
    qbMatrix2(int nRows, int nCols, const std::vector<T>* inputdata);

    // Destructor
    ~qbMatrix2();

    // Configuration methods
    bool resize(int numRows, int numCols);
    void SettoIdentity();

    // Element access methods
    T GetElement(int row, int col) const;
    bool SetElement(int row, int col, const T& value);
    int GetNumRows() const;
    int GetNumCols() const;

    // Manipulation methods
    bool Inverse(); // Not yet implemented fully

    // Comparison
    bool operator==(const qbMatrix2<T>& rhs);
    bool Compare(const qbMatrix2<T>& matrix1, double tolerance);

    // Operator overloads
    template <class U> friend qbMatrix2<U> operator+(const qbMatrix2<U>& lhs, const qbMatrix2<U>& rhs);
    template <class U> friend qbMatrix2<U> operator+(const U& lhs, const qbMatrix2<U>& rhs);
    template <class U> friend qbMatrix2<U> operator+(const qbMatrix2<U>& lhs, const U& rhs);

    template <class U> friend qbMatrix2<U> operator-(const qbMatrix2<U>& lhs, const qbMatrix2<U>& rhs);
    template <class U> friend qbMatrix2<U> operator-(const qbMatrix2<U>& lhs, const U& rhs);
    template <class U> friend qbMatrix2<U> operator-(const U& lhs, const qbMatrix2<U>& rhs);

    template <class U> friend qbMatrix2<U> operator*(const qbMatrix2<U>& lhs, const qbMatrix2<U>& rhs);
    template <class U> friend qbMatrix2<U> operator*(const U& lhs, const qbMatrix2<U>& rhs);
    template <class U> friend qbMatrix2<U> operator*(const qbMatrix2<U>& lhs, const U& rhs);

    bool Seperate(qbMatrix2<T>* matrix1, qbMatrix2<T>* matrix2, int colNum);

private:
    int Sub2Ind(int row, int col) const;
    bool IsSquare();
    bool CloseEnough(T f1, T f2);
    bool SwapRow(int i, int j);
    void MultAdd(int i, int j, T multFactor);
    void RowMult(int i, T multFactor);
    bool Join(const qbMatrix2<T>& matrix2);
    int FindRowWithMaxElement(int colNumber, int StartingRow);
    void PrintMatrix();

    T* m_matrix_Data;
    int m_nRows, m_nCols, m_nElements;
};

// ---------------- Constructors & Destructor ---------------- //

template<class T>
qbMatrix2<T>::qbMatrix2() {
    m_nRows = 1;
    m_nCols = 1;
    m_nElements = 1;
    m_matrix_Data = new T[m_nElements];
    m_matrix_Data[0] = 0.0;
}

template<class T>
qbMatrix2<T>::qbMatrix2(int nRows, int nCols) {
    m_nRows = nRows;
    m_nCols = nCols;
    m_nElements = m_nRows * m_nCols;
    m_matrix_Data = new T[m_nElements];
    for (int i = 0; i < m_nElements; i++)
        m_matrix_Data[i] = 0.0;
}

template<class T>
qbMatrix2<T>::qbMatrix2(int nRows, int nCols, const T* inputData) {
    m_nRows = nRows;
    m_nCols = nCols;
    m_nElements = m_nRows * m_nCols;
    m_matrix_Data = new T[m_nElements];
    for (int i = 0; i < m_nElements; i++)
        m_matrix_Data[i] = inputData[i];
}

template<class T>
qbMatrix2<T>::qbMatrix2(const qbMatrix2<T>& inputMatrix) {
    m_nRows = inputMatrix.m_nRows;
    m_nCols = inputMatrix.m_nCols;
    m_nElements = inputMatrix.m_nElements;
    m_matrix_Data = new T[m_nElements];
    for (int i = 0; i < m_nElements; i++)
        m_matrix_Data[i] = inputMatrix.m_matrix_Data[i];
}

template<class T>
qbMatrix2<T>::qbMatrix2(int nRows, int nCols, const std::vector<T>* inputdata) {
    m_nRows = nRows;
    m_nCols = nCols;
    m_nElements = m_nRows * m_nCols;
    m_matrix_Data = new T[m_nElements];
    for (int i = 0; i < m_nElements; i++)
        m_matrix_Data[i] = inputdata->at(i);
}

template <class T>
qbMatrix2<T>::~qbMatrix2() {
    if (m_matrix_Data != nullptr) {
        delete[] m_matrix_Data;
        m_matrix_Data = nullptr;
    }
}

// ---------------- Configuration Methods ---------------- //

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
// ---------------- Overloading Operators ----------//

// matrix + matrix
template <class T> 
qbMatrix2<T> operator+ (const qbMatrix2<T>& lhs, const qbMatrix2<T>& rhs){
int numRows = lhs.m_nRows;
int numCols = lhs.m_nCols;
int numElements = numRows*numCols;
T *tempResult = new T[numElements];
for(int i=0;i<numElements;i++)
#pragma HLS PIPELINE
	tempResult = lhs.m_matrix_Data[i] + rhs.m_matrix_Data[i];

qbMatrix2<T> result(numRows,numCols,tempResult);
delete[] tempResult;
return result;
}

//scalar + matrix

template<class T>
qbMatrix2<T> operator+ (const T& lhs,const qbMatrix2<T>& rhs){
	int numRows = rhs.m_nRows;
	int numCols = rhs.m_nCols;
	int num_Elements = numCols*numRows;
	T *tempResult = new T[num_Elements];
	for(int i=0 ; i< num_Elements; i++){
#pragma HLS PIPELINE
tempResult[i] = lhs + rhs.m_matrix_Data[i];
	}
qbMatrix2<T> result(numRows, numCols, tempResult);
delete [] tempResult;
return result;
}

//matrix + scalar
template<class T> qbMatrix2<T> operator+ (const qbMatrix2<T>& lhs,const T& rhs){
	int numRows = rhs.m_nRows;
	int numCols = rhs.m_nCols;
	int num_Elements = numCols*numRows;
	T *tempResult = new T[num_Elements];
	for(int i=0 ; i< num_Elements; i++){
#pragma HLS PIPELINE
tempResult[i] = lhs.m_matrix_Data[i] + rhs;
	}
qbMatrix2<T> result(numRows, numCols, tempResult);
delete [] tempResult;
return result;
}

// matrix - matrix
template <class T> qbMatrix2<T> operator- (const qbMatrix2<T>& lhs, const qbMatrix2<T>& rhs){
int numRows = lhs.m_nRows;
int numCols = lhs.m_nCols;
int numElements = numRows * numCols;
T *tempResult = new T[numElements];
for(int i=0 ; i<numElements ; i++)
#pragma HLS PIPELINE
	tempResult = lhs.m_matrix_Data[i] - rhs.m_matrix_Data[i];

qbMatrix2<T> result(numRows,numCols,tempResult);
delete[] tempResult;
return result;
}

//scalar - matrix
template<class T> qbMatrix2<T> operator- (const T& lhs,const qbMatrix2<T>& rhs){
	int numRows = rhs.m_nRows;
	int numCols = rhs.m_nCols;
	int num_Elements = numCols*numRows;
	T *tempResult = new T[num_Elements];
	for(int i=0 ; i< num_Elements; i++){
#pragma HLS PIPELINE
tempResult[i] = lhs - rhs.m_matrix_Data[i];
	}
qbMatrix2<T> result(numRows, numCols, tempResult);
delete [] tempResult;
return result;
}

//matrix - scalar
template<class T> qbMatrix2<T> operator- (const qbMatrix2<T>& lhs,const T& rhs){
	int numRows = rhs.m_nRows;
	int numCols = rhs.m_nCols;
	int num_Elements = numCols*numRows;
	T *tempResult = new T[num_Elements];
	for(int i=0 ; i< num_Elements; i++){
#pragma HLS PIPELINE
tempResult[i] = lhs.m_matrix_Data[i] - rhs;
	}
qbMatrix2<T> result(numRows, numCols, tempResult);
delete [] tempResult;
return result;
}

//matrix * scalar
template <class T> qbMatrix2<T> operator* (const qbMatrix2<T>& lhs, const T& rhs){
int numRows = lhs.m_nRows;
int numCols = lhs.m_nCols;
int numElements = numRows*numCols;
T *tempResult = new T[numElements];
for(int i=0;i<numElements;i++)
#pragma HLS PIPELINE
	tempResult = lhs.m_matrix_Data[i] * rhs;

qbMatrix2<T> result(numRows,numCols,tempResult);
delete[] tempResult;
return result;
}

//scalar * matrix
template <class T> qbMatrix2<T> operator* (const T& lhs, const qbMatrix2<T>& rhs){
int numRows = rhs.m_nRows;
int numCols = rhs.m_nCols;
int numElements = numRows*numCols;
T *tempResult = new T[numElements];
for(int i=0;i<numElements;++i)
#pragma HLS PIPELINE
	tempResult = lhs * rhs.m_matrix_Data[i];

qbMatrix2<T> result(numRows,numCols,tempResult);
delete[] tempResult;
return result;
}


//elementResult += (lhs.m_matrix_Data[lhsLinearIndex] * rhs.m_matrix_Data[rhsLinearIndex]);
//matrix * matrix
template <class T>
qbMatrix2<T> operator*(const qbMatrix2<T>& lhs, const qbMatrix2<T>& rhs) {
    int r_numRows = rhs.m_nRows;
    int r_numCols = rhs.m_nCols;
    int l_numRows = lhs.m_nRows;
    int l_numCols = lhs.m_nCols;
    if (l_numCols == r_numRows) {
        T* tempResult = new T[lhs.m_nRows * rhs.m_nCols];
        for (int lhsRow = 0; lhsRow < l_numRows; lhsRow++) {
#pragma HLS PIPELINE
            for (int rhsCol = 0; rhsCol < r_numCols; rhsCol++) {
                T elementResult = T(0);
                for (int lhsCol = 0; lhsCol < l_numCols; lhsCol++) {
#pragma HLS UNROLL
                    int lhsLinearIndex = (lhsRow * l_numCols) + lhsCol;
                    int rhsLinearIndex = (lhsCol * r_numCols) + rhsCol;
                    elementResult += (lhs.m_matrix_Data[lhsLinearIndex] * rhs.m_matrix_Data[rhsLinearIndex]);
                }
                int resultLinearIndex = (lhsRow * r_numCols) + rhsCol;
                tempResult[resultLinearIndex] = elementResult;
            }
        }
        qbMatrix2<T> result(l_numRows, r_numCols, tempResult);
        delete[] tempResult;
        return result;
    } else {
        qbMatrix2<T> result(1, 1);
        return result;
    }
}

// the == operator overloading. 
template <class T>
bool qbMatrix2<T>::operator==(const qbMatrix2<T>& rhs) {
    // Check if the matrices are of same size, if not return false.
    if ((this->m_nRows != rhs.m_nRows) && (this->m_nCols != rhs.m_nCols))
        return false;
    // Check if the elements are equal
    bool flag = true;
    for (int i = 0; i < this->m_nElements; ++i) {
        if (!CloseEnough(this->m_matrix_Data[i], rhs.m_matrix_Data[i]))
            flag = false;
    }
    return flag;
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
bool qbMatrix2<T>::IsSquare() { 
    if(m_nRows == m_nCols){
        return true;
    } else {
        return false;
    }
}

template<class T>
bool qbMatrix2<T>::CloseEnough(T f1, T f2) {
    return std::fabs(f1 - f2) < 1e-9;
}

template<class T>
bool qbMatrix2<T>::Seperate(qbMatrix2<T> *matrix1, qbMatrix2<T> *matrix2, int colNum) {
    // Compute the sizes of the new matrices.
    int numRows = m_nRows;
    int numCols1 = colNum;
    int numCols2 = m_nCols - colNum;
    // resize the matrix to fit the new sizes
    matrix1->resize(numRows, numCols1);
    matrix2->resize(numRows, numCols2);
    // loop over the original matrix, and copy the elements to the new matrices
    for (int row = 0; row < m_nRows; ++row) {
        for (int col = 0; col < m_nCols; ++col) {
            if (col < colNum) {
                matrix1->SetElement(row, col, this->GetElement(row, col));
            } else {
                matrix2->SetElement(row, col - colNum, this->GetElement(row, col));
            }
        }
    }
    return true;
}

template <class T>
delete[] newMatrixData;
bool qbMatrix2<T>::Join(const qbMatrix2<T>& matrix2) {
    int numRows1 = m_nRows;
    int numRows2 = matrix2.m_nRows;
    int numCols1 = m_nCols;
    int numCols2 = matrix2.m_nCols;
    if (numRows1 != numRows2) {
        throw std::invalid_argument("Matrices must have the same number of rows to join");
    }
    // Allocate the memory for the result
    // Note that only the number of columns increases.
    T* newMatrixData = new T[numRows1 * (numCols1 + numCols2)];
    // copy the two matrices into the new matrix
    int linearIndex, resultLinearIndex;
    for (int row = 0; row < numRows1; ++row) {
        for (int col = 0; col < (numCols1 + numCols2); ++col) {
            resultLinearIndex = row * (numCols1 + numCols2) + col;
            if (col < numCols1) {
                linearIndex = (row * numCols1) + col;
                newMatrixData[resultLinearIndex] = m_matrix_Data[linearIndex];
            } else {
                linearIndex = (row * numCols2) + (col - numCols1);
                newMatrixData[resultLinearIndex] = matrix2.m_matrix_Data[linearIndex];
            }
        }
    }
    m_nCols = numCols1 + numCols2;
    m_nElements = m_nRows * m_nCols;
    delete[] m_matrix_Data;
    m_matrix_Data = new T[m_nElements];
    for (int i = 0; i < m_nElements; ++i)
        m_matrix_Data[i] = newMatrixData[i];
    delete[] newMatrixData;
    return true;
}

template <class T>
int qbMatrix2<T>::SwapRow(int i,int j){
 
    //Store a temporary copy of row i
    T* tempRow = new T[m_nCols];

    for(int k=0 ; k<m_nCols ; ++k){
        tempRow[k] = m_matrix_Data[Sub2Ind(i,k)];
    }
    for(int k=0 ; k<m_nCols ; ++k){
        m_matrix_Data[Sub2Ind(i,k)] = m_matrix_Data[Sub2Ind(j,k)];
    }
    for(int k=0 ; k<m_nCols ; ++k){
        m_matrix_Data[Sub2Ind(j,k)] = tempRow[k];
    }
    //basic swap temp = a; a = b; b = temp;
    delete[] tempRow;
}

template <class T>
void qbMatrix2<T>::MultAdd(int i, int j, T multFactor){
    //Multiply row j by multFactor and add to row i
    for(int col=0 ; col<m_nCols ; ++col){
        m_matrix_Data[Sub2Ind(i,col)] += (multFactor * m_matrix_Data[Sub2Ind(j,col)]);
    }
}

template <class T>
void qbMatrix2<T>::RowMult(int i, T multFactor){
    for(int col=0 ; col<m_nCols ; ++col){
        m_matrix_Data[Sub2Ind(i,col)] *= multFactor;
    }
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
//just lifted this code out of the test code.....

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
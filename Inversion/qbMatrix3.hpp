#ifndef QBMATRIX2_H
#define QBMATRIX2_H

#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <vector>

// Template class qbMatrix2 for generic matrix handling
// Provides constructors, destructors, element access, resize, identity, comparison,
// operator overloading, and utility functions for linear algebra-like operations.

template <class T>
class qbMatrix2 {
public:
    // Constructors
    qbMatrix2();
    qbMatrix2(int nRow, int nCol);
    qbMatrix2(int nRow, int nCol, const T* inputdata);
    qbMatrix2(const qbMatrix2<T>& inputMatrix);
    qbMatrix2(int nRows, int nCols, const std::vector<T> *inputdata);

        // Destructor
    ~qbMatrix2();

    bool resize(int numRows, int numCols);
    void SettoIdentity();
    T GetElement(int row, int col);
    bool SetElement(int row, int col, const T& value);
    int GetNumRows();
    int GetNumCols();   
    
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
public:
    int Sub2Ind(int row, int col) const;
    bool IsSquare();
    bool CloseEnough(T f1, T f2);
    void SwapRow(int i, int j);
    void MultAdd(int i, int j, T multFactor);
    void RowMult(int i, T multFactor);
    bool Join(const qbMatrix2<T>& matrix2);
    int FindRowWithMaxElement(int colNumber, int StartingRow);
    void PrintMatrix();

private: //can be used public as testing else its private.
    T* m_matrix_Data;
    int m_nRows, m_nCols, m_nElements;
};
//*****************************************************************************//
/////////////////////////////Constructor and Destructor//////////////////////////

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
//Construct the linear array from the input 2D array.

template<class T>
qbMatrix2<T>::qbMatrix2(int nRows, int nCols, const T* inputData) {
    m_nRows = nRows;
    m_nCols = nCols;
    m_nElements = m_nRows * m_nCols;
    m_matrix_Data = new T[m_nElements];
    for (int i = 0; i < m_nElements; i++)
        m_matrix_Data[i] = inputData[i];
}

//the coppy constructor
template<class T>
qbMatrix2<T>::qbMatrix2(const qbMatrix2<T>& inputMatrix) {
    m_nRows = inputMatrix.m_nRows;
    m_nCols = inputMatrix.m_nCols;
    m_nElements = inputMatrix.m_nElements;
    m_matrix_Data = new T[m_nElements];
    for (int i = 0; i < m_nElements; i++)
        m_matrix_Data[i] = inputMatrix.m_matrix_Data[i];
}

template <class T>
qbMatrix2<T>::~qbMatrix2() {
    if (m_matrix_Data != nullptr) {
        delete[] m_matrix_Data;
        m_matrix_Data = nullptr;
    }
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


 /*********************************************************/
 // Configuration Methods
 /*********************************************************/

 template <class T>
bool qbMatrix2<T>::resize(int numRows, int numCols) {
    m_nRows = numRows;
    m_nCols = numCols;
    m_nElements = m_nRows * m_nCols;
    delete[] m_matrix_Data;
    m_matrix_Data = new T[m_nElements];
    if (m_matrix_Data != nullptr) 
    {
        for (int i = 0; i < m_nElements; i++)
            m_matrix_Data[i] = 0.0;
        return true;
    } else {
        return false;
    }
}

template<class T>
void qbMatrix2<T>::SettoIdentity() {
    if (!IsSquare())
    throw std::runtime_error("Identity only for square matrices");

    for (int i = 0; i < m_nRows; i++)
        for (int j = 0; j < m_nCols; j++)
            m_matrix_Data[Sub2Ind(i, j)] = (i == j) ? 1.0 : 0.0;
}
// -
 //****************************************************** */
//Element Access Methods
 //****************************************************** */

 template<class T>
T qbMatrix2<T>::GetElement(int row, int col) {
    int linearIndex = Sub2Ind(row, col);
    if(linearIndex >= 0){
        return m_matrix_Data[linearIndex];
    }
    else {
        return 0.0;
    }
}

template<class T>
bool qbMatrix2<T>::SetElement(int row, int col, const T& elementValue){
    int linearIndex = Sub2Ind(row, col);
    if(linearIndex >= 0){
        m_matrix_Data[linearIndex] = elementValue;
        return true;
    } 
    else {
        return false;
    }
}

template<class T>
int qbMatrix2<T>::GetNumRows() { return m_nRows; }

template<class T>
int qbMatrix2<T>::GetNumCols() { return m_nCols; }

template<class T>
bool qbMatrix2<T>::Compare(const qbMatrix2<T>& matrix1, double tolerance) {
   //First check if the matrices are of same size, if not return false.
   int numRows = matrix1.m_nRows;
   int numCols = matrix1.m_nCols;
   if((numRows != m_nRows) || (numCols != m_nCols)){
       return false;
   }

   double cummulativeError = 0.0;
   for(int i=0; i < m_nElements; i++){
       T element1 = m_matrix_Data[i];
         T element2 = matrix1.m_matrix_Data[i];
        cummulativeError += ((element1 - element2) * (element1 - element2));
   }
   double finalValue = sqrt(cummulativeError / ((numRows * numCols) - 1));
   if(finalValue < tolerance)
         return true;
    else
         return false;  

}

template<class T>
bool qbMatrix2<T>::Seperate(qbMatrix2<T> *matrix1, qbMatrix2<T> *matrix2, int colNum) {
    // Compute the sizes of the new matrices.
    int numRows = m_nRows;
    int numCols1 = colNum;
    int numCols2 = m_nCols - colNum;
    // resize the matrix to fit the new sizes
    matrix1->resize(numRows, numCols1);
    matrix2->resize(numRows, numCols1);
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


//******************************************************** */
// Utility Functions
//******************************************************** */

template<class T>
int qbMatrix2<T>::Sub2Ind(int row, int col){
    if((row < m_nRows) && (row >= 0) && (col < m_nCols) && (col >= 0)){
        return (row * m_nCols) + col;
    }
    else{
        return -1;
    }
}

template<class T>
bool qbMatrix2<T>::CloseEnough(T f1, T f2) {
    return std::fabs(f1 - f2) < 1e-9;
}

template<class T>
bool qbMatrix2<T>::IsSquare() {
    if (m_nRows == m_nCols) {
        return true;
    } else {
        return false;
    }
}

template <class T>
void qbMatrix2<T>::RowMult(int i, T multFactor){
    for(int col=0 ; col<m_nCols ; ++col){
        m_matrix_Data[Sub2Ind(i,col)] *= multFactor;
    }
}

template <class T>
void qbMatrix2<T>::MultAdd(int i, int j, T multFactor){
    //Multiply row j by multFactor and add to row i
    for(int col=0 ; col<m_nCols ; ++col){
        m_matrix_Data[Sub2Ind(i,col)] += (multFactor * m_matrix_Data[Sub2Ind(j,col)]);
    }
}
/************************************************************** */
//find the row with the maximum element in a given column starting from a given row
//returns the row index
/************************************************************** */
template <class T>
int qbMatrix2<T>::FindRowWithMaxElement(int colNumber, int StartingRow){ 
T tempValue m_matrix_Data[Sub2Ind(StartingRow,colNumber)];
int rowIndex = StartingRow;
for(int i=StartingRow+1 ; i<m_nRows ; ++i){
    if(fabs(m_matrix_Data[Sub2Ind(i,colNumber)]) > fabs(tempValue)){
        rowIndex = i;
        tempValue = m_matrix_Data[Sub2Ind(i,colNumber)];
    }
}
return rowIndex;
}
/************************************************************** */
//Swap two rows of the matrix
//************************************************************** */
template <class T>
void qbMatrix2<T>::SwapRow(int i,int j){
 
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

/************************************************************** */
//Join two matrices horizontally
/************************************************************** */

template <class T>
bool qbMatrix2<T>::Join(const qbMatrix2<T>& matrix2) {
    int numRows1 = m_nRows;
    int numCols1 = m_nCols;
    int numRows2 = matrix2.m_nRows;
    int numCols2 = matrix2.m_nCols;

    if(numRows1 != numRows2) {
        return false; // Matrices must have the same number of rows to join
    }

    //Alocate memory for the new matrix
    T *newMatrixData = new T[(numCols1 + numCols2) * numRows1];

    int linearIndex, resultLinearIndex;
    for(int i = 0; i< numRows1; ++i) {
        for(int j = 0; j < numCols1; ++j) {
            
            resultLinearIndex = (i * (numCols1 + numCols2)) + j;
            if( j < numCols1){
            linearIndex = (i* numCols1) + j;
            newMatrixData[resultLinearIndex] = m_matrix_Data[linearIndex];
            }
            else {
                linearIndex = (i * numCols2) + (j - numCols1);
                newMatrixData[resultLinearIndex] = matrix2.m_matrix_Data[linearIndex];
            }

        }
    }

    m_nCols = numCols1 + numCols2;
    m_nElements = m_nRows * m_nCols;
    delete[] m_matrix_Data;
    m_matrix_Data = new T[m_nElements];
    for(int i = 0; i < m_nElements; ++i)
        m_matrix_Data[i] = newMatrixData[i];

    delete[] newMatrixData;
    return true;
}


/*************************************************************/
// Overloading Operators
/*************************************************************/

// ---------------- Overloading Operators ----------//

// matrix + matrix
template <class T> 
qbMatrix2<T> operator+ (const qbMatrix2<T>& lhs, const qbMatrix2<T>& rhs){
    int numRows = lhs.m_nRows;
    int numCols = lhs.m_nCols;
    int numElements = numRows*numCols;
    T *tempResult = new T[numElements];
    for(int i=0;i<numElements;i++)
        tempResult[i] = lhs.m_matrix_Data[i] + rhs.m_matrix_Data[i];
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
tempResult[i] = lhs + rhs.m_matrix_Data[i];
	}
qbMatrix2<T> result(numRows, numCols, tempResult);
delete [] tempResult;
return result;
}

//matrix + scalar
template<class T> qbMatrix2<T> operator+ (const qbMatrix2<T>& lhs,const T& rhs){
    int numRows = lhs.m_nRows;
    int numCols = lhs.m_nCols;
    int num_Elements = numCols*numRows;
    T *tempResult = new T[num_Elements];
    for(int i=0 ; i< num_Elements; i++){
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
        tempResult[i] = lhs.m_matrix_Data[i] - rhs.m_matrix_Data[i];
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
    int numRows = lhs.m_nRows;
    int numCols = lhs.m_nCols;
    int num_Elements = numCols*numRows;
    T *tempResult = new T[num_Elements];
    for(int i=0 ; i< num_Elements; i++){
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
        tempResult[i] = lhs.m_matrix_Data[i] * rhs;
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
        tempResult[i] = lhs * rhs.m_matrix_Data[i];
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
            for (int rhsCol = 0; rhsCol < r_numCols; rhsCol++) {
                T elementResult = 0.0;
                for (int lhsCol = 0; lhsCol < l_numCols; lhsCol++) {
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
// ***************************************************************/
// the equality operator
//****************************************************************/
// the == operator overloading. 

template <class T>
bool qbMatrix2<T>::operator==(const qbMatrix2<T>& rhs) {
    // Check if the matrices are of same size, if not return false. &&
    if ((this->m_nRows != rhs.m_nRows) || (this->m_nCols != rhs.m_nCols))
        return false;
    // Check if the elements are equal
    bool flag = true;
    for (int i = 0; i < this->m_nElements; ++i) {
        if (this->m_matrix_Data[i] != rhs.m_matrix_Data[i])
            flag = false;
    }
    return flag;
}
/***************************************************************** */
//Inverse of a matrix using Gauss-Jordan elimination
/***************************************************************** */

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
    while((!completeFlag) && (count < maxCount)) {
        for (int diagIndex = 0; diagIndex < m_nRows; ++diagIndex) {
            int cRow = diagIndex;
            int cCol = diagIndex;

            // Find the row with the largest element in this column
            int maxIndex = FindRowWithMaxElement(cCol, cRow);
            if (maxIndex != cRow) {
                SwapRow(cRow, maxIndex);
            }

            // Make the diagonal element 1 by multiplying the row by the inverse of the diagonal element
            if (m_matrix_Data[Sub2Ind(cRow, cCol)] != 1.0) {
                T multFactor = 1.0 / m_matrix_Data[Sub2Ind(cRow, cCol)];
                RowMult(cRow, multFactor);
            }

            // Eliminate below-diagonal elements in this column
            for (int rowIndex = cRow + 1; rowIndex < m_nRows; ++rowIndex) {
                if (!CloseEnough(m_matrix_Data[Sub2Ind(rowIndex, cCol)], 0.0)) { //elm actually 0 so skip op
                    int rowOneIndex = cCol;
                    T currentElement = m_matrix_Data[Sub2Ind(rowIndex, cCol)];
                    T rowOneValue = m_matrix_Data[Sub2Ind(rowOneIndex, cCol)];
                    if (!CloseEnough(rowOneValue, 0.0)) {
                        T correctionFactor = -(currentElement / rowOneValue);
                        MultAdd(rowIndex, rowOneIndex, correctionFactor);
                    }
                }
            }

            // Eliminate off-diagonal elements in this row
            for (int colIndex = cCol + 1; colIndex < m_nCols; ++colIndex) {
                if (!CloseEnough(m_matrix_Data[Sub2Ind(cRow, colIndex)], 0.0)) {
                    int rowOneIndex = colIndex;
                    T currentElement = m_matrix_Data[Sub2Ind(cRow, colIndex)];
                    T rowOneValue = m_matrix_Data[Sub2Ind(rowOneIndex, colIndex)];
                    if (!CloseEnough(rowOneValue, 0.0)) {
                        T correctionFactor = -(currentElement / rowOneValue);
                        MultAdd(cRow, rowOneIndex, correctionFactor);
                    }
                }
            }
        }

        // Separate the result into the left and the right halves
        qbMatrix2<T> leftHalf;
        qbMatrix2<T> rightHalf;
        this->Seperate(&leftHalf, &rightHalf, originalNumCols);

        // When the process is complete, the left half should be the identity matrix, and the right half should be the inverse
        if (leftHalf == identityMatrix) {
            completeFlag = true;
            // Rebuild this matrix to be the right half
            m_nCols = originalNumCols;
            m_nElements = m_nRows * m_nCols;
            delete[] m_matrix_Data;
            m_matrix_Data = new T[m_nElements];
            for (int i = 0; i < m_nElements; ++i) {
                m_matrix_Data[i] = rightHalf.m_matrix_Data[i];
            }
        }
        count++; // Always increment count to avoid infinite loop
    }
    return completeFlag;
}

#endif
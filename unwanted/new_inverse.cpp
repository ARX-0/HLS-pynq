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
    bool operator==(const qbMatrix2<T>& rhs); // we need change this to see the thereshold level like not the absolute value
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

private: //can be used public as testing else its private.
    int Sub2Ind(int row, int col) const;
    bool IsSquare();
    bool CloseEnough(T f1, T f2);
    void SwapRow(int i, int j);
    void MultAdd(int i, int j, T multFactor);
    void RowMult(int i, T multFactor);
    bool Join(const qbMatrix2<T>& matrix2);
    int FindRowWithMaxElement(int colNumber, int StartingRow);
    void PrintMatrix();

    T *m_matrix_Data;
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
   // if (numRows < 1 || numCols < 1) return false;
   // delete[] m_matrix_Data;
    m_nRows = numRows;
    m_nCols = numCols;
    m_nElements = m_nRows * m_nCols;
    delete[] m_matrix_Data;
    m_matrix_Data = new T[m_nElements];
    if(matrix_Data == nullptr){
    for (int i = 0; i < m_nElements; i++)
        m_matrix_Data[i] = 0.0;
    return true;
}
else{
    return false;
}
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
T qbMatrix2<T>::GetElement(int row, int col) {
    int linearIndex = Sub2Ind(row, col);
    if (linearIndex >= 0) {
        return m_matrix_Data[linearIndex];
    } else {
        return 0.0;
    }
}

template<class T>
bool qbMatrix2<T>::SetElement(int row, int col, const T value) {
int linearindex = Sub2Ind(row, col);
if(linearindex >= 0){
    m_matrix_Data[linearindex] = value;
    return true;
}
else{
    return false;
    }
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
}
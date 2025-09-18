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

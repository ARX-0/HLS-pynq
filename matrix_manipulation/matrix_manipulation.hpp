#ifndef QBMATRIX2_H
#define QBMATRIX2_H

#define N 4 //change the size here

// NOTE: This file preserves your original structure and naming,
// but replaces unsupported dynamic allocation with a fixed-size
// 1D storage array (m_matrix_Data). Each place that previously used
// new[]/delete[] is commented as BEFORE and replaced with an AFTER
// implementation that is HLS friendly (no heap).

// -------------------------------
// qbMatrix2 - header (OOP, 1D storage)
// -------------------------------
template <class T>
class qbMatrix2
{
public:
    // Constructors
    qbMatrix2();
    qbMatrix2(int nRow, int nCol);
    qbMatrix2(int nRow, int nCol, const T* inputdata);
    qbMatrix2(const qbMatrix2<T>& inputMatrix);  // Pass by const reference

    // Destructor
    ~qbMatrix2();

    // Configuration method
    bool resize(int numRows, int numCols);

    // Element access methods
    T GetElement(int row, int col) const;
    bool SetElement(int row, int col, const T& value);
    int GetNumRows() const;
    int GetNumCols() const;

    // Comparison operator
    bool operator==(const qbMatrix2<T>& rhs) const;

    // Overload +, -, * operators (friends)
    template <class U> friend qbMatrix2<U> operator+(const qbMatrix2<U>& lhs, const qbMatrix2<U>& rhs);
    template <class U> friend qbMatrix2<U> operator+(const U& lhs, const qbMatrix2<U>& rhs);
    template <class U> friend qbMatrix2<U> operator+(const qbMatrix2<U>& lhs, const U& rhs);

    template <class U> friend qbMatrix2<U> operator-(const qbMatrix2<U>& lhs, const qbMatrix2<U>& rhs);
    template <class U> friend qbMatrix2<U> operator-(const U& lhs, const qbMatrix2<U>& rhs);
    template <class U> friend qbMatrix2<U> operator-(const qbMatrix2<U>& lhs, const U& rhs);

    template <class U> friend qbMatrix2<U> operator*(const qbMatrix2<U>& lhs, const qbMatrix2<U>& rhs);
    template <class U> friend qbMatrix2<U> operator*(const U& lhs, const qbMatrix2<U>& rhs);
    template <class U> friend qbMatrix2<U> operator*(const qbMatrix2<U>& lhs, const U& rhs);

private:
    int Sub2Ind(int row, int col) const;

    // -------------------------
    // BEFORE: dynamic allocation (unsupported by HLS)
    //    T* m_matrix_Data;
    // AFTER: fixed-size 1D storage (HLS-compliant)
    T m_matrix_Data[N * N];
    // -------------------------

    int m_nRows, m_nCols, m_nElements;
};

// -------------------------------
// CONSTRUCTION AND DESTRUCTOR
// -------------------------------

// create an empty matrix
template<class T>
qbMatrix2<T>::qbMatrix2() {
    m_nRows = 1;
    m_nCols = 1;
    m_nElements = 1;
    // BEFORE:
    //    m_matrixData = new T[m_nElements];
    //    m_matrixData[0] = 0.0;
    // AFTER: use fixed storage
    m_matrix_Data[0] = (T)0;
}

// construct rows x cols (zero-initialized)
template<class T>
qbMatrix2<T>::qbMatrix2(int nRows, int nCols) {
    m_nRows = nRows;
    m_nCols = nCols;
    m_nElements = m_nRows * m_nCols;
    // BEFORE:
    //    m_matrixData = new T[m_nElements];
    //    for (int i=0; i<m_nElements; ++i) m_matrixData[i] = 0.0;
    // AFTER: fill fixed array (up to used elements)
    for (int i = 0; i < m_nElements; ++i) {
#pragma HLS PIPELINE
        m_matrix_Data[i] = (T)0;
    }
}

// construct from linear input array
template<class T>
qbMatrix2<T>::qbMatrix2(int nRows, int nCols, const T *inputData) {
    m_nRows = nRows;
    m_nCols = nCols;
    m_nElements = m_nRows * m_nCols;
    // BEFORE:
    //    m_matrixData = new T[m_nElements];
    //    for(int i=0;i < m_nElements;i++ ){
    //        m_matrixData[i] = inputData[i];
    //    }
    // AFTER:
    for (int i = 0; i < m_nElements; ++i) {
#pragma HLS PIPELINE
        m_matrix_Data[i] = inputData[i];
    }
}

// copy constructor
template<class T>
qbMatrix2<T>::qbMatrix2(const qbMatrix2<T>& inputMatrix) {
    m_nRows = inputMatrix.m_nRows;
    m_nCols = inputMatrix.m_nCols;
    m_nElements = inputMatrix.m_nElements;
    // BEFORE:
    //    m_matrixData = new T[m_nElements];
    //    for(int i=0; i<m_nElements;i++){
    //        m_matrixData[i] = inputMatrix.m_matrix_Data[i];
    //    }
    // AFTER:
    for (int i = 0; i < m_nElements; ++i) {
#pragma HLS PIPELINE
        m_matrix_Data[i] = inputMatrix.m_matrix_Data[i];
    }
}

// Destructor.
// BEFORE: destructor deleted dynamic memory (unsupported)
// template <class T>
// qbMatrix2<T>::~qbMatrix2(){
//     if(m_matrix_data != nullptr){
//         delete[] m_matrixData;
//     }
// }
// AFTER: trivial destructor (no heap)
template <class T>
qbMatrix2<T>::~qbMatrix2() {
    // no dynamic memory to free
}

// -------------------------------
// CONFIG FUNCTIONS
// -------------------------------

template <class T>
bool qbMatrix2<T>::resize(int numRows, int numCols) {
    m_nRows = numRows;
    m_nCols = numCols;
    m_nElements = m_nRows * m_nCols;

    // BEFORE:
    //    delete[] m_matrixData;
    //    m_matrixData = new T[m_nElements];
    //    if(m_matrixData != nullptr){
    //        for(int i=0; i<m_nElements;i++){
    //            m_matrixData[i] = 0.0;
    //            return true;
    //        }
    //    } else { return false; }
    // AFTER: ensure requested size fits fixed storage
    if (m_nElements <= (N * N)) {
        for (int i = 0; i < m_nElements; ++i) {
#pragma HLS PIPELINE
            m_matrix_Data[i] = (T)0;
        }
        return true;
    } else {
        // requested size too large for fixed storage
        return false;
    }
}

// -------------------------------
// ELEMENT FUNCTIONS
// -------------------------------

template <class T>
T qbMatrix2<T>::GetElement(int row, int col) const {
    int linearIndex = Sub2Ind(row, col);
    if (linearIndex >= 0) {
        return m_matrix_Data[linearIndex];
    } else {
        return (T)0;
    }
}

template <class T>
bool qbMatrix2<T>::SetElement(int row, int col, const T& elementValue) {
    int linearIndex = Sub2Ind(row, col);
    if (linearIndex >= 0) {
        m_matrix_Data[linearIndex] = elementValue;
        return true;
    } else {
        return false;
    }
}

template <class T>
int qbMatrix2<T>::GetNumRows() const { return m_nRows; }

template <class T>
int qbMatrix2<T>::GetNumCols() const { return m_nCols; }

// -------------------------------
// OPERATOR OVERLOADS (no dynamic alloc)
// -------------------------------

// matrix + matrix
template <class T>
qbMatrix2<T> operator+ (const qbMatrix2<T>& lhs, const qbMatrix2<T>& rhs) {
    int numRows = lhs.m_nRows;
    int numCols = lhs.m_nCols;
    qbMatrix2<T> result(numRows, numCols);

    // BEFORE:
    //    int numElements = numRows*numCols;
    //    T *tempResult = new T[numElements];
    //    for(int i=0;i<numElements;i++)
    //    #pragma HLS PIPELINE
    //        tempResult = lhs.m_matrix_Data[i] + rhs.m_matrix_Data[i];
    //    qbMatrix2<T> result(numRows,numCols,tempResult);
    //    delete[] tempResult;
    // AFTER: write directly into result storage
    int numElements = numRows * numCols;
    for (int i = 0; i < numElements; ++i) {
#pragma HLS PIPELINE
        result.m_matrix_Data[i] = lhs.m_matrix_Data[i] + rhs.m_matrix_Data[i];
    }
    return result;
}

// scalar + matrix
template<class T>
qbMatrix2<T> operator+ (const T& lhs, const qbMatrix2<T>& rhs) {
    int numRows = rhs.m_nRows;
    int numCols = rhs.m_nCols;
    qbMatrix2<T> result(numRows, numCols);

    // BEFORE: tempResult = new ...
    // AFTER:
    int numElements = numRows * numCols;
    for (int i = 0; i < numElements; ++i) {
#pragma HLS PIPELINE
        result.m_matrix_Data[i] = lhs + rhs.m_matrix_Data[i];
    }
    return result;
}

// matrix + scalar
template<class T>
qbMatrix2<T> operator+ (const qbMatrix2<T>& lhs, const T& rhs) {
    int numRows = lhs.m_nRows;
    int numCols = lhs.m_nCols;
    qbMatrix2<T> result(numRows, numCols);

    int numElements = numRows * numCols;
    for (int i = 0; i < numElements; ++i) {
#pragma HLS PIPELINE
        result.m_matrix_Data[i] = lhs.m_matrix_Data[i] + rhs;
    }
    return result;
}

// matrix - matrix
template <class T>
qbMatrix2<T> operator- (const qbMatrix2<T>& lhs, const qbMatrix2<T>& rhs) {
    int numRows = lhs.m_nRows;
    int numCols = lhs.m_nCols;
    qbMatrix2<T> result(numRows, numCols);

    int numElements = numRows * numCols;
    for (int i = 0; i < numElements; ++i) {
#pragma HLS PIPELINE
        result.m_matrix_Data[i] = lhs.m_matrix_Data[i] - rhs.m_matrix_Data[i];
    }
    return result;
}

// scalar - matrix
template<class T>
qbMatrix2<T> operator- (const T& lhs, const qbMatrix2<T>& rhs) {
    int numRows = rhs.m_nRows;
    int numCols = rhs.m_nCols;
    qbMatrix2<T> result(numRows, numCols);

    int numElements = numRows * numCols;
    for (int i = 0; i < numElements; ++i) {
#pragma HLS PIPELINE
        result.m_matrix_Data[i] = lhs - rhs.m_matrix_Data[i];
    }
    return result;
}

// matrix - scalar
template<class T>
qbMatrix2<T> operator- (const qbMatrix2<T>& lhs, const T& rhs) {
    int numRows = lhs.m_nRows;
    int numCols = lhs.m_nCols;
    qbMatrix2<T> result(numRows, numCols);

    int numElements = numRows * numCols;
    for (int i = 0; i < numElements; ++i) {
#pragma HLS PIPELINE
        result.m_matrix_Data[i] = lhs.m_matrix_Data[i] - rhs;
    }
    return result;
}

// matrix * scalar
template <class T>
qbMatrix2<T> operator* (const qbMatrix2<T>& lhs, const T& rhs) {
    int numRows = lhs.m_nRows;
    int numCols = lhs.m_nCols;
    qbMatrix2<T> result(numRows, numCols);

    int numElements = numRows * numCols;
    for (int i = 0; i < numElements; ++i) {
#pragma HLS PIPELINE
        result.m_matrix_Data[i] = lhs.m_matrix_Data[i] * rhs;
    }
    return result;
}

// scalar * matrix
template <class T>
qbMatrix2<T> operator* (const T& lhs, const qbMatrix2<T>& rhs) {
    int numRows = rhs.m_nRows;
    int numCols = rhs.m_nCols;
    qbMatrix2<T> result(numRows, numCols);

    int numElements = numRows * numCols;
    for (int i = 0; i < numElements; ++i) {
#pragma HLS PIPELINE
        result.m_matrix_Data[i] = lhs * rhs.m_matrix_Data[i];
    }
    return result;
}

// matrix * matrix
template <class T>
qbMatrix2<T> operator* (const qbMatrix2<T>& lhs, const qbMatrix2<T>& rhs) {
    int r_numRows = rhs.m_nRows;
    int r_numCols = rhs.m_nCols;
    int l_numRows = lhs.m_nRows;
    int l_numCols = lhs.m_nCols;

    // BEFORE:
    //    T *tempResult = new T[lhs.m_nRows * rhs.m_nCols];
    //    ... fill tempResult ...
    //    qbMatrix2<T> result(l_numRows,r_numCols,tempResult);
    //    delete[] tempResult;
    // AFTER: write directly to result.m_matrix_Data

    qbMatrix2<T> result(l_numRows, r_numCols);

//#pragma HLS ARRAY_PARTITION variable=lhs.m_matrix_Data cyclic factor=4 dim=1
//#pragma HLS ARRAY_PARTITION variable=rhs.m_matrix_Data cyclic factor=4 dim=1

    if (l_numCols == r_numRows) {

        // loop through each row of the LHS
        for (int lhsRow = 0; lhsRow < l_numRows; lhsRow++) {

            // loop through each column on the RHS
            for (int rhsCol = 0; rhsCol < r_numCols; rhsCol++) {
#pragma HLS PIPELINE II=1
                T elementResult = (T)0;
                // Loop through each element in the LHS row
                for (int lhsCol = 0; lhsCol < l_numCols; lhsCol++) {
#pragma HLS UNROLL factor = 4
                    int lhsLinearIndex = (lhsRow * l_numCols) + lhsCol;
                    int rhsLinearIndex = (lhsCol * r_numCols) + rhsCol; // corrected stride
                    elementResult += (lhs.m_matrix_Data[lhsLinearIndex] * rhs.m_matrix_Data[rhsLinearIndex]);
                }
                int resultLinearIndex = (lhsRow * r_numCols) + rhsCol;
                result.m_matrix_Data[resultLinearIndex] = elementResult;
            }
        }

        result.m_nRows = l_numRows;
        result.m_nCols = r_numCols;
        result.m_nElements = l_numRows * r_numCols;
        return result;
    } else {
        qbMatrix2<T> fallback(1, 1);
        fallback.m_matrix_Data[0] = (T)0;
        fallback.m_nElements = 1;
        return fallback;
    }
}

// -------------------------------
// Comparison operator
// -------------------------------
/*template <class T>
bool qbMatrix2<T>::operator== (const qbMatrix2<T> rhs) {
    if ((this->m_nRows != rhs.m_nRows) || (this->m_nCols != rhs.m_nCols))
        return false;

    for (int i = 0; i < this->m_nElements; ++i) {
        if (this->m_matrix_Data[i] != rhs.m_matrix_Data[i])
            return false;
    }
    return true;
}
*/
// -------------------------------
// Sub2Ind helper (1D indexing)
// -------------------------------
template <class T>
int qbMatrix2<T>::Sub2Ind(int row, int col) const {
    if ((row < m_nRows) && (row >= 0) && (col < m_nCols) && (col >= 0)) {
        return (row * m_nCols) + col;
    } else
        return -1;
}

#endif // QBMATRIX2_H

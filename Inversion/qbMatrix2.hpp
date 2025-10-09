#ifndef QBMATRIX2_H
#define QBMATRIX2_H

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

        // Destructor
    ~qbMatrix2();

    bool resize(int numRows, int numCols);

    T GetElement(int row, int col);
    bool SetElement(int row, int col, const T& value);
    int GetNumRows();
    int GetNumCols();   
    
    // Comparison
    bool operator==(const qbMatrix2<T>& rhs);    

    
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

private:
    int Sub2Ind(int row, int col);

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


#endif
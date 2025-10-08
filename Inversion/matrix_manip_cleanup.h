
#ifndef QBMATRIX2_H
#define QBMATRIX2_H
#include <vector>
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <iomanip>

template <class T>
class qbMatrix2
{
public:
    // Constructors
    qbMatrix2();
    qbMatrix2(int nRow, int nCol);
	qbMatrix2(int nRow, int nCol, const T* inputdata);
	qbMatrix2(const qbMatrix2<T>& inputMatrix);  // Pass by const reference
	qbMatrix2(int nRows, int nCols, const std::vector<T>* inputdata);

    // Destructor
    ~qbMatrix2();

    // Configuration method
    bool resize(int numRows, int numCols);
    void SettoIdentity();

    // Element access methods
    T GetElement(int row, int col) const;
    bool SetElement(int row, int col, const T& value);
    int GetNumRows() const;
    int GetNumCols() const;

    //manipulation methods
    //compute the matrix inverse with the Gauss Gordan method.
    bool Inverse();

    // Comparison operator
    bool operator==(const qbMatrix2<T>& rhs); //changes
    bool Compare (const qbMatrix2<T>& matrix1, double tolerance);

    // Overload +, -, * operators (friends)
    template <class U> friend qbMatrix2<U> operator+(const qbMatrix2<U>& lhs, const qbMatrix2<U>& rhs);
    template <class U> friend qbMatrix2<U> operator+(const U& lhs, const qbMatrix2<U>& rhs);
    template <class U> friend qbMatrix2<U> operator+(const qbMatrix2<U>& lhs, const U& rhs);

    template <class U> friend qbMatrix2<U> operator-(const qbMatrix2<U>& lhs, const qbMatrix2<U>& rhs);
    template <class U> friend qbMatrix2<U> operator-(const qbMatrix2<U>& lhs, const U& rhs);
    template <class U> friend qbMatrix2<U> operator-(const U& lhs, const qbMatrix2<U>& rhs);

    template <class U> friend qbMatrix2<U> operator*(const qbMatrix2<U>& lhs, const qbMatrix2<U>& rhs);
    template <class U> friend qbMatrix2<U> operator*(const U& lhs, const qbMatrix2<U>& rhs);
    template <class U> friend qbMatrix2<U> operator*(const qbMatrix2<U>& lhs, const U& rhs);

	bool Seperate(qbMatrix2<T> *matrix1, qbMatrix2<T> *matrix2, int colNum);

    //qbMatrix2<T> operator+ (const qbMatrix2<T>& lhs, const qbMatrix2<T>& rhs
    //qbMatrix2<T> operator+ (const T& lhs,const qbMatrix2<T>& rhs)
    //qbMatrix2<T> operator+ (const qbMatrix2<T>& lhs,const T& rhs)

private:
    int Sub2Ind(int row, int col) const;
    bool IsSquare();
    bool CloseEnough(T f1, T f2);
    bool SwapRow(int i, int j);
    void MultRow(int i, int j, T multFactor);
	bool Join(const qbMatrix2<T>& matrix2);
	int FindRowWithMaxElement(int colNumber, int StartingRow);
	void PrintMatrix();

    T* m_matrix_Data;
    int m_nRows, m_nCols, m_nElements;
};

//THE CONSTRUCTION AND DESTRUCTOR FUNCTIONS//

//create an empty matrix
template<class T>
qbMatrix2<T>::qbMatrix2() {
    m_nRows = 1;
    m_nCols = 1;
    m_nElements = 1;
    m_matrix_Data = new T[m_nElements];
    m_matrix_Data[0] = T(0);
}

// construct a linear array
template<class T> qbMatrix2<T>::qbMatrix2(int nRows,int nCols, const T *inputData){
	m_nRows = nRows;
	m_nCols = nCols;
	m_nElements = m_nRows * m_nCols;
	m_matrix_Data = new T[m_nElements];
	for(int i=0;i < m_nElements;i++ ){
		m_matrix_Data[i] = inputData[i];
	}
}

//The copy constructor
template<class T>
qbMatrix2<T>::qbMatrix2(const qbMatrix2<T>& inputMatrix){
	m_nRows = inputMatrix.m_nRows;
	m_nCols = inputMatrix.m_nCols;
	m_nElements = inputMatrix.m_nElements;
	m_matrix_Data = new T[m_nElements];
	for(int i=0; i<m_nElements;i++){
		m_matrix_Data[i] = inputMatrix.m_matrix_Data[i];
	}
}
//new std:vector
template<class T>
qbMatrix2<T>::qbMatrix2(int nRows, int nCols, const std::vector<T> *inputdata) {
	m_nRows = nRows;
	m_nCols = nCols;
	m_nElements = m_nRows * m_nCols;
	m_matrix_Data = new T[m_nElements];
	for (int i = 0; i < m_nElements; i++) {
		m_matrix_Data[i] = inputdata->at(i);
	}
}

//Destructor.
template <class T>
qbMatrix2<T>::~qbMatrix2() {
	if (m_matrix_Data != nullptr) {
		delete[] m_matrix_Data;
	}
}
///////////conig fns////////////

template <class T>
bool qbMatrix2<T>::resize(int numRows, int numCols) {
	m_nRows = numRows;
	m_nCols = numCols;
	m_nElements = m_nRows * m_nCols;
	delete[] m_matrix_Data;
	m_matrix_Data = new T[m_nElements];
	if (m_matrix_Data != nullptr) {
		for (int i = 0; i < m_nElements; i++) {
			m_matrix_Data[i] = T(0);
		}
		return true;
	} else {
		return false;
	}
}
///////element functions///////

template <class T>
void qbMatrix2<T>::SettoIdentity() {
	if (!IsSquare()) {
		throw std::invalid_argument("Cannot form an I matrix as its not a square matrix");
	}
	for (int row = 0; row < m_nRows; ++row) {
		for (int col = 0; col < m_nCols; ++col) {
			if (col == row)
				m_matrix_Data[Sub2Ind(row, col)] = T(1);
			else
				m_matrix_Data[Sub2Ind(row, col)] = T(0);
		}
	}
}

template <class T>
T qbMatrix2<T>::GetElement(int row, int col) const {
	int linearIndex = Sub2Ind(row, col);
	if (linearIndex >= 0) {
		return m_matrix_Data[linearIndex];
	} else {
		return T(0);
	}
	else
		return 0.0;
}

template <class T>
bool qbMatrix2<T>::SetElement(int row, int col, const T& value) {
	int linearIndex = Sub2Ind(row, col);
	if (linearIndex >= 0) {
		m_matrix_Data[linearIndex] = value;
		return true;
	} else {
		return false;
	}
}

template <class T>
int qbMatrix2<T>::GetNumRows() const { return m_nRows; }
template <class T>
int qbMatrix2<T>::GetNumCols() const { return m_nCols; }

template <class T>
bool qbMatrix2<T>::Compare(const qbMatrix2<T>& matrix1, double tolerance) {
	int numRows1 = matrix1.m_nRows;
	int numCols1 = matrix1.m_nCols;
	if ((numRows1 != m_nRows) || (numCols1 != m_nCols)) {
		return false;
	}
	double cummulativeSum = 0.0;
	for (int i = 0; i < m_nElements; ++i) {
		T element1 = matrix1.m_matrix_Data[i];
		T element2 = m_matrix_Data[i];
		cummulativeSum += ((element1 - element2) * (element1 - element2));
	}
	double finalValue = sqrt(cummulativeSum / ((numRows1 * numCols1) - 1));
	if (finalValue < tolerance) {
		return true;
	} else {
		return false;
	}
}

////Overloading the operators//////


// matrix + matrix
template <class T> qbMatrix2<T> operator+ (const qbMatrix2<T>& lhs, const qbMatrix2<T>& rhs){
int numRows = lhs.m_nRows;
int numCols = lhs.m_nCols;
int numElements = numRows*numCols;
T *tempResult = new T[numElements];
for(int i=0;i<numElements;i++)
//#pragma HLS PIPELINE
	tempResult = lhs.m_matrix_Data[i] + rhs.m_matrix_Data[i];

qbMatrix2<T> result(numRows,numCols,tempResult);
delete[] tempResult;
return result;
}

//scalar + matrix

template<class T> qbMatrix2<T> operator+ (const T& lhs,const qbMatrix2<T>& rhs){
	int numRows = rhs.m_nRows;
	int numCols = rhs.m_nCols;
	int num_Elements = numCols*numRows;
	T *tempResult = new T[num_Elements];
	for(int i=0 ; i< num_Elements; i++){
//#pragma HLS PIPELINE
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
//#pragma HLS PIPELINE
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
int numElements = numRows*numCols;
T *tempResult = new T[numElements];
for(int i=0;i<numElements;i++)
//#pragma HLS PIPELINE
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
//#pragma HLS PIPELINE
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
//#pragma HLS PIPELINE
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
//#pragma HLS PIPELINE
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
//#pragma HLS PIPELINE
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
//#pragma HLS PIPELINE
            for (int rhsCol = 0; rhsCol < r_numCols; rhsCol++) {
                T elementResult = T(0);
                for (int lhsCol = 0; lhsCol < l_numCols; lhsCol++) {
//#pragma HLS UNROLL
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

template <class T>
bool qbMatrix2<T>::operator==(const qbMatrix2<T>& rhs) {
    // Check if the matrices are of same size
    if ((this->m_nRows != rhs.m_nRows) || (this->m_nCols != rhs.m_nCols))
        return false;
    // Check if the elements are equal
    bool flag = true;
    for (int i = 0; i < this->m_nElements; ++i) {
        if (!CloseEnough(this->m_matrix_Data[i], rhs.m_matrix_Data[i]))
            flag = false;
    }
    return flag;
}

// the private class Sub2Ind
template <class T>
int qbMatrix2<T>::Sub2Ind(int row, int col) const {
	if ((row < m_nRows) && (row >= 0) && (col < m_nCols) && (col >= 0)) {
		return (row * m_nCols) + col;
	} else {
		return -1;
	}
}
#endif

bool qbMatrix2<T>::CloseEnough(T f1, T f2) {
	return std::fabs(f1 - f2) < 1e-9;
}

template <class T> bool qbMatrix2<T>::Seperate(qbMatrix2<T> *matrix1 , qbMatrix2<T> *matrix2, int colNum){
int numRows = m_nRows;
int numCols1 = m_nCols;
int numCols2 = m_nCols - colNum;

matrix1->resize(numRows,numCols1);
matrix2->resize(numRows,numCols2);

for (int row = 0 ; row < m_nRows; ++row){

	for(int col = 0; col < m_nCols ;++col){
		if(col <colNum)
		{
			matrix1->SetElement(row, col, this->GetElement(row, col));
		}
		else
		{
			matrix2->SetElement(row, col-colNum, this->GetElement(row,col));
		}
	}
}
return true; // what is this?
}

template <class T>
bool qbMatrix2<T>::Join(const qbMatrix2<T>& matrix2){

	int numRows1 = m_nRows;
	int numRows2 = matrix2.m_nRows;
	int numCols1 = m_nCols;
	int numCols2 = matrix2.m_nCols;
if(numRows1 != numRows2)
	throw std::invalid_argument("Attempt to join matrices with different numbers of rows is invalid");

	//Allocate memory for the result
	//Only the numbers of columns increases
	T* newmatrixdata = new T [numRows1 * (numCols1+numCols2)];

	int linearIndex, resultLinearIndex;
	int numCols = numCols1 + numCols2;

for(int i=0;i<numRows1;i++){
	for(int j=0;j<numCols;j++){
		resultLinearIndex =(i *numCols)+j;
		//if j is in the left hand matrix we get the data from there;
		if(j<numCols1){
			linearIndex = (i *numCols1)+j;
			newmatrixdata[resultLinearIndex] = m_matrix_Data[linearIndex];
		}
		// Otherwise, j must be in the right hand matrix, so we get the data from there...
		else{
			linearIndex = (i*numCols2)+ (j - numCols1) ;
			newmatrixdata[resultLinearIndex] = matrix2.m_matrix_Data[linearIndex];
			}
		}
	}

m_nCols = numCols;
m_nElements = m_nRows * m_nCols;
delete[] m_matrix_Data;
m_matrix_Data = new T[m_nElements];
for(int i=0 ; i<m_nElements ; ++i){
	m_matrix_Data[i] = newMatrixData[i];

	delete newMatrixData;
	return true;
}

}

template <class T> bool qbMatrix2<T>::isSquare(){
if(m_nRows == m_nCols){ return true;}
else {return false;}
}

template <class T> void qbMatrix2<T>::SwapRow(int i,int j){
	T *tempRow = new T[m_nCols];

//store temp copy of row i
	for(int k=0;k<m_nCols;++k){
		tempRow[k] = m_matrix_Data[Sub2Ind(i,k)];
	}

//replace i with row j
	for(int k=0;k<m_nCols;++k){
		m_matrix_Data[Sub2Ind[i,k]] = m_matrix_Data[Sub2Ind(j,k)];
	}

//replace row k with temp copy of the original row i
	for(int k=0;k<m_nCols;++k){
		m_matrix_Data[Sub2Ind(j,k)] = tempRow[k];
	}

	delete[] tempRow;
}

template <class T>::qbMatrix2<T>::MultAdd(int i, int j, T multFactor){
	for(int k=0; k<m_nCols;++k){
		m_matrix_Data[Sub2Ind(i,k)] += (m_matrix_Data[Sub2Ind(j,k)] * multFactor);
	}
}

template<class T>void qbMatrix2<T>::RowMult(int i, T Multfactor){

for(int k=0; k<=m_nCols;++k){
	m_matrix_Data[Sub2Ind(i,k)] *= Multfactor;
}
}

template<class T> void qbMatrix2<T>::PrintMatrix(){
	int nRows = this->GetNumRows();
	int nCols = this->GetNumCols();
	for(int row = 0 ; row <nRows ; ++row){
		for(int col = 0 ; col <nCols ;++cols){
			std::cout << std::fixed << std::setprecision(3) << this->GetElement(row, col) << " ";
		}
		std::cout << std::endl;
	}
}

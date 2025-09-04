#ifndef QBMATRIX2_H
#define QBMATRIX2_H

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
    int Sub2Und(int row, int col) const;

    T* m_matrix_Data;
    int m_nRows, m_nCols, m_nElements;
};

//THE CONSTRUCTION AND DESTRUCTOR FUNCTIONS//

template<class T>
qbMatrix2<T>::qbmatrix2(){
	m_nRows = 1;
	m_nCols = 1;
	m_nElements = 1;
	m_matrixData = new T[m_nElements];
	m_matrixData[0] = 0.0;
}
template<class T> qbMatrix2<T>::qbMatrix2(int nRows,int nCols, const T *inputData){
	m_nRows = nRows;
	m_nCols = nCols;
	m_nElements = m_nRows * m_nCols;
	m_matrixData = new T[m_nElements];
	for(int i=0;i < m_nElements;i++ ){
		m_matrixData[i] = inputData[i];
	}
}

//The copy constructor

template<class T>
qbMatrix2<T>::qbMatrix2(const qbMatrix2<T>& inputMatrix){
	m_nRows = inputMatrix.m_nRows;
	m_nCols = inputMatrix.m_nCols;
	m_nElements = inputMatrix.m_nElements;
	m_matrixData = new T[m_nElements];
	for(int i=0; i<m_nElements;i++){
		m_matrixData[i] = inputMatrix.m_matrix_Data[i];
	}
}

template <class T>
qbMatrix2<T>::~qbmatrix2(){
if(m_matrix_data != nullptr){
	delete[] m_matrixData;
}
}
template <class T>
bool qbMatrix2<T>::resize(int numRows,int numCols){
m_nRows = numRows;
m_nCols = numCols;
delete[] m_matrixData;
m_matrixData = new T[m_nElements];

if(m_matrixData != nullptr){
	for(int i=0; i<m_nElements;i++){
		m_matrixData[i] = 0.0;
		return true;
	}
}
else {
	return false;
	}
}

template <class T> T qbMatrix2<T>::GetElement(int row,int col){
	int linearIndex = Sub2Ind(row,col);
	if(linearIndex >= 0){
		return m_matrixData[linearIndex];
	}
	else
		return 0.0;
}

template <class T> bool qbMatrix2<T>::SetElement(int row, int col, T elementValue){
	int linearIndex = Sub2Ind(row, col);
	if(linearIndex >= 0){
		m_matrixData[linearIndex] = elementValue;
		return true;
	}
	else{
		return false;
	}
}
////Overloading the operators//////

// matrix + matrix
template <class T> qbMatrix2<T> operator+ (const qbMatrix2<T>& lhs, const qbMatrix<T>& rhs){
int numRows = lhs.m_nRows;
int numCols = lhs.m_nCols;
int numElements = numRows*numCols;
T *tempResult = new T[numElements];
for(int i=0;i<numElements;i++)
#pragma HLS PIPELINE
	tempResult = lhs.m_matrixData[i] + rhs.m_matrixData[i];

qbMatrix2<T> result(numRows,numCols,tempResult);
delete[] tempResult;
return result;
}

//scalar + matrix

template<class T> qbMatrix2<T> operator+ (const T& lhs,const qbMatrix2<T>& rhs){
	int numRows = rhs.m_nRows;
	int numCols = rhs.m_nCols;
	int num_Elements = numCols*numRows;
	T *tempResult = new T[numElement];
	for(int i=0 ; i< num_Elements; i++){
#pragma HLS PIPELINE
tempResult[i] = lhs + rhs.m_matrix_Data[i];
	}
qbMatrix2<T> result(numRows, numCols, tempResult);
delete [] tempResult;
return result;
}

template <class T> int qbMatrix2<T>::GetCols(){}

template <class T> int qbMatrix2<T>::Sub2Ind(int row, int col){
	if((row < m_nRows) && (row >= 0) && (col < m_nCols) && (col >=0)){
       return (row * m_nCols) + col ;
}
	else
		return -1;
}
#endif

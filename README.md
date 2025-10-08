# HLS-pynq
this repo gives you a progressive view upon making optimisations on HLS and implementation on the PYNQ-Z2 board

## C simulation result

![image](https://github.com/user-attachments/assets/69ba6f18-bc25-4b99-a807-5e56fce6d6ef)

Execution time (sec) = clock cycles * clocl periood
                     = clock cycles / clock frequency
                     500Mhz PS clock and 250Mhz PL clock in the Pynq-Z2 board

![image](https://github.com/user-attachments/assets/2b8a8eb9-c0c3-49ac-ad87-a455185c966e)

1 function call of fir requires 29 clock cycles and 108ns (unoptimised)
1 whole clock cycle to call a function so interval is Latency+1 that is 30
fir_test.cpp has 600 samples that means 600 calls of that function

600 * 30 = 18000 cycles to complete the function 

C similation for  testbench and RTL simulation for the kernel file (cycle accurate simulation)

### Co-simulation report

![image](https://github.com/user-attachments/assets/b2bae0b7-9a6d-4060-a680-75e8286ebdfb)

its different than that of the synthesis report 
Max and Avg latncy being 27 and Min II being 28 

600 * 28 = 16,800

### START (ap_start=1)

![image](https://github.com/user-attachments/assets/654516d1-ebdf-484a-adbf-76446e6abda4)


### END (ap_done=0)

![image](https://github.com/user-attachments/assets/559e77f0-c569-498c-9db2-ccf978413b74)


600 calls of th fir function ap_done is 600

see timing of starting and ending 67314-114 ns = 67.2us (which is 16,800 cycles)

## optmistaions 

![image](https://github.com/user-attachments/assets/621350ed-5226-49c8-9889-842cad93d04b)

### can be optimsed as 

![image](https://github.com/user-attachments/assets/8c89b3a1-d327-45ee-8556-bb4f5adaee3b)

this is called code refactoring (seperating 0 case from the first case and going forward)


# Matrix multiplication before and after optimisation 

## What we need to know 

![image](https://github.com/user-attachments/assets/9c06175e-fd44-4351-ab4c-a5650321a543)

A = [ [ a00 a01 a02 a03 a04 a05 a06 a07 ]   // Row 0
      [ a10 a11 a12 a13 a14 a15 a16 a17 ]   // Row 1
      [ a20 a21 a22 a23 a24 a25 a26 a27 ]   // Row 2
      [ a30 a31 a32 a33 a34 a35 a36 a37 ] ] // Row 3

### In Block partioning  (type=block dim=1 factor=4)

Partition 0: a00 a01 a02 a03     | Partition 1: a04 a05 a06 a07
Partition 0: a10 a11 a12 a13     | Partition 1: a14 a15 a16 a17
Partition 0: a20 a21 a22 a23     | Partition 1: a24 a25 a26 a27
Partition 0: a30 a31 a32 a33     | Partition 1: a34 a35 a36 a37

### In cyclic patrioning (type=cyclic dim=1 factor=4)

Partition 0: a00 a04     | Partition 1: a01 a05     | Partition 2: a02 a06     | Partition 3: a03 a07
Partition 0: a10 a14     | Partition 1: a11 a15     | Partition 2: a12 a16     | Partition 3: a13 a17
Partition 0: a20 a24     | Partition 1: a21 a25     | Partition 2: a22 a26     | Partition 3: a23 a27
Partition 0: a30 a34     | Partition 1: a31 a35     | Partition 2: a32 a36     | Partition 3: a33 a37

### In complete partitioning (type=complete dim=1)

Row 0: [ a00 ][ a01 ][ a02 ][ a03 ][ a04 ][ a05 ][ a06 ][ a07 ]
Row 1: [ a10 ][ a11 ][ a12 ][ a13 ][ a14 ][ a15 ][ a16 ][ a17 ]
Row 2: [ a20 ][ a21 ][ a22 ][ a23 ][ a24 ][ a25 ][ a26 ][ a27 ]
Row 3: [ a30 ][ a31 ][ a32 ][ a33 ][ a34 ][ a35 ][ a36 ][ a37 ]

![image](https://github.com/user-attachments/assets/49919201-d0a7-4bf1-842f-b0eff10fc47c)

## Before
![image](https://github.com/user-attachments/assets/3bb7cfb0-aecc-4cbe-ac38-9338bda90ae4)

![image](https://github.com/user-attachments/assets/2b59cd6b-773e-4938-b8d7-fbbd4975c65b)

## After

![image](https://github.com/user-attachments/assets/fff5a384-9cf0-452c-9196-a264a4f498df)

![image](https://github.com/user-attachments/assets/4fca4248-0970-46ad-b159-95c5dc855477)


/////////////////////////////////////////////


In file included from testcode.cpp:11:0:
qbMatrix.h: In function ‘bool Compare(const qbMatrix2<T>&, double)’:
qbMatrix.h:353:21: error: ‘m_nRows’ was not declared in this scope
     if((numRows1 != m_nRows) || (numCols1 != m_nCols)){
                     ^~~~~~~
qbMatrix.h:353:21: note: suggested alternative: ‘numRows1’
     if((numRows1 != m_nRows) || (numCols1 != m_nCols)){
                     ^~~~~~~
                     numRows1
qbMatrix.h:353:46: error: ‘m_nCols’ was not declared in this scope
     if((numRows1 != m_nRows) || (numCols1 != m_nCols)){
                                              ^~~~~~~
qbMatrix.h:353:46: note: suggested alternative: ‘numCols1’
     if((numRows1 != m_nRows) || (numCols1 != m_nCols)){
                                              ^~~~~~~
                                              numCols1
qbMatrix.h:361:22: error: ‘m_matrix_Data’ was not declared in this scope
         T element2 = m_matrix_Data[i];
                      ^~~~~~~~~~~~~
qbMatrix.h:364:30: error: ‘commulativeSum’ was not declared in this scope
     double finalValue = sqrt(commulativeSum / ((numRows1 * numCols1) - 1));
                              ^~~~~~~~~~~~~~
qbMatrix.h:364:30: note: suggested alternative: ‘cummulativeSum’
     double finalValue = sqrt(commulativeSum / ((numRows1 * numCols1) - 1));
                              ^~~~~~~~~~~~~~
                              cummulativeSum
qbMatrix.h:374:1: error: a template declaration cannot appear at block scope
 template <class T> //to find the greatest element in a row
 ^~~~~~~~
qbMatrix.h:397:1: error: a template declaration cannot appear at block scope
 template<class T>
 ^~~~~~~~
qbMatrix.h:411:1: error: a template declaration cannot appear at block scope
 template<class T>
 ^~~~~~~~
qbMatrix.h:434:24: error: qualified-id in declaration before ‘(’ token
 bool qbMatrix2<T>::Join(const qbMatrix2<T>& matrix2) {
                        ^
qbMatrix.h:494:1: error: a template declaration cannot appear at block scope
 template <class T>
 ^~~~~~~~
qbMatrix.h:521:1: error: a template declaration cannot appear at block scope
 template <class T>
 ^~~~~~~~
qbMatrix.h:542:27: error: qualified-id in declaration before ‘(’ token
 bool qbMatrix2<T>::Inverse() {
                           ^
testcode.cpp:26:12: error: a function-definition is not allowed here before ‘{’ token
 int main() {
            ^
testcode.cpp:145:1: error: expected ‘}’ at end of input
 }

 //The error log 2
 ////////////////////////////////////////////////////////////
 g++ -o testcode testcode.cpp -std=c++17
In file included from testcode.cpp:11:0:
qbMatrix.h:1:0: error: unterminated #ifndef
 #ifndef QBMATRIX2_H
 
In file included from testcode.cpp:11:0:
qbMatrix.h: In function ‘bool Compare(const qbMatrix2<T>&, double)’:
qbMatrix.h:353:21: error: ‘m_nRows’ was not declared in this scope
     if((numRows1 != m_nRows) || (numCols1 != m_nCols)){
                     ^~~~~~~
qbMatrix.h:353:21: note: suggested alternative: ‘numRows1’
     if((numRows1 != m_nRows) || (numCols1 != m_nCols)){
                     ^~~~~~~
                     numRows1
qbMatrix.h:353:46: error: ‘m_nCols’ was not declared in this scope
     if((numRows1 != m_nRows) || (numCols1 != m_nCols)){
                                              ^~~~~~~
qbMatrix.h:353:46: note: suggested alternative: ‘numCols1’
     if((numRows1 != m_nRows) || (numCols1 != m_nCols)){
                                              ^~~~~~~
                                              numCols1
qbMatrix.h:361:22: error: ‘m_matrix_Data’ was not declared in this scope
         T element2 = m_matrix_Data[i];
                      ^~~~~~~~~~~~~
qbMatrix.h:364:30: error: ‘commulativeSum’ was not declared in this scope
     double finalValue = sqrt(commulativeSum / ((numRows1 * numCols1) - 1));
                              ^~~~~~~~~~~~~~
qbMatrix.h:364:30: note: suggested alternative: ‘cummulativeSum’
     double finalValue = sqrt(commulativeSum / ((numRows1 * numCols1) - 1));
                              ^~~~~~~~~~~~~~
                              cummulativeSum
qbMatrix.h:375:1: error: a template declaration cannot appear at block scope
 template <class T> //to find the greatest element in a row
 ^~~~~~~~
qbMatrix.h:399:1: error: a template declaration cannot appear at block scope
 template<class T>
 ^~~~~~~~
qbMatrix.h:413:1: error: a template declaration cannot appear at block scope
 template<class T>
 ^~~~~~~~
qbMatrix.h:436:24: error: qualified-id in declaration before ‘(’ token
 bool qbMatrix2<T>::Join(const qbMatrix2<T>& matrix2) {
                        ^
testcode.cpp:145:1: error: expected ‘}’ at end of input
 }
 ^
//////////////3 problems////////////////////////////////////////////

g++ -o testcode testcode.cpp -std=c++17
In file included from testcode.cpp:10:0:
qbMatrix.h:393:1: error: expected unqualified-id before ‘delete’
 delete[] newMatrixData;
 ^~~~~~
qbMatrix.h:539:16: error: ‘T’ was not declared in this scope
 bool qbMatrix2<T>::Inverse() {
                ^
qbMatrix.h:539:17: error: template argument 1 is invalid
 bool qbMatrix2<T>::Inverse() {
                 ^
qbMatrix.h: In function ‘bool Inverse()’:
qbMatrix.h:540:9: error: ‘IsSquare’ was not declared in this scope
     if(!IsSquare()) {
         ^~~~~~~~
qbMatrix.h:547:15: error: ‘T’ was not declared in this scope
     qbMatrix2<T> identityMatrix(m_nRows, m_nCols);
               ^
qbMatrix.h:547:16: error: template argument 1 is invalid
     qbMatrix2<T> identityMatrix(m_nRows, m_nCols);
                ^
qbMatrix.h:547:33: error: ‘m_nRows’ was not declared in this scope
     qbMatrix2<T> identityMatrix(m_nRows, m_nCols);
                                 ^~~~~~~
qbMatrix.h:547:42: error: ‘m_nCols’ was not declared in this scope
     qbMatrix2<T> identityMatrix(m_nRows, m_nCols);
                                          ^~~~~~~
qbMatrix.h:547:49: error: expression list treated as compound expression in initializer [-fpermissive]
     qbMatrix2<T> identityMatrix(m_nRows, m_nCols);
                                                 ^
qbMatrix.h:548:20: error: request for member ‘SettoIdentity’ in ‘identityMatrix’, which is of non-class type ‘int’
     identityMatrix.SettoIdentity();
                    ^~~~~~~~~~~~~
qbMatrix.h:552:5: error: ‘Join’ was not declared in this scope
     Join(identityMatrix);
     ^~~~
qbMatrix.h:552:5: note: suggested alternative: ‘sin’
     Join(identityMatrix);
     ^~~~
     sin
qbMatrix.h:564:28: error: ‘FindRowWithMaxElement’ was not declared in this scope
             int maxIndex = FindRowWithMaxElement(cCol, cRow);
                            ^~~~~~~~~~~~~~~~~~~~~
qbMatrix.h:566:17: error: ‘SwapRow’ was not declared in this scope
                 SwapRow(cRow, maxIndex);
                 ^~~~~~~
qbMatrix.h:570:17: error: ‘m_matrix_Data’ was not declared in this scope
             if (m_matrix_Data[Sub2Ind(cRow, cCol)] != 0) {
                 ^~~~~~~~~~~~~
qbMatrix.h:570:31: error: ‘Sub2Ind’ was not declared in this scope
             if (m_matrix_Data[Sub2Ind(cRow, cCol)] != 0) {
                               ^~~~~~~
qbMatrix.h:571:19: error: expected ‘;’ before ‘multFactor’
                 T multFactor = 1.0 / m_matrix_Data[Sub2Ind(cRow, cCol)];
                   ^~~~~~~~~~
qbMatrix.h:572:31: error: ‘multFactor’ was not declared in this scope
                 RowMult(cRow, multFactor);
                               ^~~~~~~~~~
qbMatrix.h:572:17: error: ‘RowMult’ was not declared in this scope
                 RowMult(cRow, multFactor);
                 ^~~~~~~
qbMatrix.h:577:34: error: ‘m_matrix_Data’ was not declared in this scope
                 if (!CloseEnough(m_matrix_Data[Sub2Ind(rowIndex, cCol)], 0.0)) {
                                  ^~~~~~~~~~~~~
qbMatrix.h:577:48: error: ‘Sub2Ind’ was not declared in this scope
                 if (!CloseEnough(m_matrix_Data[Sub2Ind(rowIndex, cCol)], 0.0)) {
                                                ^~~~~~~
qbMatrix.h:577:22: error: ‘CloseEnough’ was not declared in this scope
                 if (!CloseEnough(m_matrix_Data[Sub2Ind(rowIndex, cCol)], 0.0)) {
                      ^~~~~~~~~~~
qbMatrix.h:579:23: error: expected ‘;’ before ‘currentElement’
                     T currentElement = m_matrix_Data[Sub2Ind(rowIndex, cCol)];
                       ^~~~~~~~~~~~~~
qbMatrix.h:580:23: error: expected ‘;’ before ‘rowOneValue’
                     T rowOneValue = m_matrix_Data[Sub2Ind(rowOneIndex, cCol)];
                       ^~~~~~~~~~~
qbMatrix.h:581:38: error: ‘rowOneValue’ was not declared in this scope
                     if (!CloseEnough(rowOneValue, 0.0)) {
                                      ^~~~~~~~~~~
qbMatrix.h:581:38: note: suggested alternative: ‘rowOneIndex’
                     if (!CloseEnough(rowOneValue, 0.0)) {
                                      ^~~~~~~~~~~
                                      rowOneIndex
qbMatrix.h:582:27: error: expected ‘;’ before ‘correctionFactor’
                         T correctionFactor = -(currentElement / rowOneValue);
                           ^~~~~~~~~~~~~~~~
qbMatrix.h:583:56: error: ‘correctionFactor’ was not declared in this scope
                         MultAdd(rowIndex, rowOneIndex, correctionFactor);
                                                        ^~~~~~~~~~~~~~~~
qbMatrix.h:583:25: error: ‘MultAdd’ was not declared in this scope
                         MultAdd(rowIndex, rowOneIndex, correctionFactor);
                         ^~~~~~~
qbMatrix.h:590:34: error: ‘m_matrix_Data’ was not declared in this scope
                 if (!CloseEnough(m_matrix_Data[Sub2Ind(cRow, colIndex)], 0.0)) {
                                  ^~~~~~~~~~~~~
qbMatrix.h:590:48: error: ‘Sub2Ind’ was not declared in this scope
                 if (!CloseEnough(m_matrix_Data[Sub2Ind(cRow, colIndex)], 0.0)) {
                                                ^~~~~~~
qbMatrix.h:590:22: error: ‘CloseEnough’ was not declared in this scope
                 if (!CloseEnough(m_matrix_Data[Sub2Ind(cRow, colIndex)], 0.0)) {
                      ^~~~~~~~~~~
qbMatrix.h:592:23: error: expected ‘;’ before ‘currentElement’
                     T currentElement = m_matrix_Data[Sub2Ind(cRow, colIndex)];
                       ^~~~~~~~~~~~~~
qbMatrix.h:593:23: error: expected ‘;’ before ‘rowOneValue’
                     T rowOneValue = m_matrix_Data[Sub2Ind(rowOneIndex, colIndex)];
                       ^~~~~~~~~~~
qbMatrix.h:594:38: error: ‘rowOneValue’ was not declared in this scope
                     if (!CloseEnough(rowOneValue, 0.0)) {
                                      ^~~~~~~~~~~
qbMatrix.h:594:38: note: suggested alternative: ‘rowOneIndex’
                     if (!CloseEnough(rowOneValue, 0.0)) {
                                      ^~~~~~~~~~~
                                      rowOneIndex
qbMatrix.h:595:27: error: expected ‘;’ before ‘correctionFactor’
                         T correctionFactor = -(currentElement / rowOneValue);
                           ^~~~~~~~~~~~~~~~
qbMatrix.h:596:52: error: ‘correctionFactor’ was not declared in this scope
                         MultAdd(cRow, rowOneIndex, correctionFactor);
                                                    ^~~~~~~~~~~~~~~~
qbMatrix.h:596:25: error: ‘MultAdd’ was not declared in this scope
                         MultAdd(cRow, rowOneIndex, correctionFactor);
                         ^~~~~~~
qbMatrix.h:603:20: error: type/value mismatch at argument 1 in template parameter list for ‘template<class T> class qbMatrix2’
         qbMatrix2<T> leftHalf;
                    ^
qbMatrix.h:603:20: note:   expected a type, got ‘T’
qbMatrix.h:604:20: error: type/value mismatch at argument 1 in template parameter list for ‘template<class T> class qbMatrix2’
         qbMatrix2<T> rightHalf;
                    ^
qbMatrix.h:604:20: note:   expected a type, got ‘T’
qbMatrix.h:605:9: error: invalid use of ‘this’ in non-member function
         this->Seperate(&leftHalf, &rightHalf, originalNumCols);
         ^~~~
qbMatrix.h:612:13: error: ‘m_nElements’ was not declared in this scope
             m_nElements = m_nRows * m_nCols;
             ^~~~~~~~~~~
qbMatrix.h:613:22: error: ‘m_matrix_Data’ was not declared in this scope
             delete[] m_matrix_Data;
                      ^~~~~~~~~~~~~
qbMatrix.h:614:33: error: ‘T’ does not name a type
             m_matrix_Data = new T[m_nElements];
                                 ^
qbMatrix.h:616:46: error: request for member ‘m_matrix_Data’ in ‘rightHalf’, which is of non-class type ‘int’
                 m_matrix_Data[i] = rightHalf.m_matrix_Data[i];
                                              ^~~~~~~~~~~~~
testcode.cpp: In function ‘int main()’:
testcode.cpp:95:40: error: ‘void qbMatrix2<T>::PrintMatrix() [with T = double]’ is private within this context
             currentMatrix->PrintMatrix();
                                        ^
In file included from testcode.cpp:10:0:
qbMatrix.h:528:6: note: declared private here
 void qbMatrix2<T>::PrintMatrix() {
      ^~~~~~~~~~~~





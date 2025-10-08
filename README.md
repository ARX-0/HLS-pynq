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




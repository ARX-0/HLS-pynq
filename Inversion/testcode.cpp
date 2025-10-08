#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <math.h>
#include <sstream>
#include <fstream>

#include "matrix_manip_cleanup.h"

using namespace std;

// ---------------- Print ---------------- //
/*
template<class T>
void qbMatrix2<T>::PrintMatrix() {
    for (int i = 0; i < m_nRows; i++) {
        for (int j = 0; j < m_nCols; j++)
            std::cout << std::setw(10) << m_matrix_Data[Sub2Ind(i, j)] << " ";
        std::cout << "\n";
    }
}
*/
int main() {
    std::string filename = "test_matrices.csv";

    /*Openfile*/
    ifstream input(filename);
    if(input.is_open()){
        //the file has one header line containing the number of rows and columns of each matrix
        std::string currline;
        getline(input, currline);

        // Accept both "# n" and "n" as header
        int nRows = 0;
        size_t spacepos = currline.find(" ");
        if (spacepos != std::string::npos) {
            std::string nrows_str = currline.substr(spacepos, currline.length());
            nRows = atoi(nrows_str.c_str());
        } else {
            nRows = atoi(currline.c_str());
        }

        std::stringstream ss;
        double currentNumber;
        std::vector<double> currentRow;
        std::vector<double> currentMatrixData;
        std::vector<qbMatrix2<double>*> failedMatrices;
        int rowCount = 0;
        int numSuccess = 0;
        int numFailures = 0;

        // Use getline(input, currline) as loop condition
        while (getline(input, currline)) {
            ss << currline;
            while (ss >> currentNumber) {
                currentRow.push_back(currentNumber);
                if (ss.peek() == ',') {
                    ss.ignore();
                }
            }
            ss.clear();
            currentMatrixData.insert(currentMatrixData.end(), currentRow.begin(), currentRow.end());
            currentRow.clear();
            rowCount++;

            // Use nRows for matrix size
            if (rowCount == nRows) {
                rowCount = 0;
                if (currentMatrixData.size() == static_cast<size_t>(nRows * 2 * nRows)) {
                    qbMatrix2<double> newMatrix(nRows, 2 * nRows, &currentMatrixData);
                    qbMatrix2<double> leftMatrix;
                    qbMatrix2<double> rightMatrix;
                    newMatrix.Seperate(&leftMatrix, &rightMatrix, nRows);
                    leftMatrix.Inverse();
                    if (leftMatrix.Compare(rightMatrix, 1e-9)) {
                        numSuccess++;
                        std::cout << "Test passed" << std::endl;
                    } else {
                        numFailures++;
                        std::cout << "Test failed" << std::endl;
                        failedMatrices.push_back(new qbMatrix2<double>(newMatrix));
                    }
                } else {
                    std::cerr << "Malformed input: expected " << nRows * 2 * nRows << " elements, got " << currentMatrixData.size() << std::endl;
                }
                currentMatrixData.clear();
            }
        }

        std::cout << "Num success = " << numSuccess << std::endl;
        std::cout << "Num failures = " << numFailures << std::endl;
        for (qbMatrix2<double>* currentMatrix : failedMatrices) {
            currentMatrix->PrintMatrix();
            delete currentMatrix;
        }
        input.close();
        return 0;
    } else {
        std::cerr << "Could not open file: " << filename << std::endl;
        return 1;
    }
}


/*
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <math.h>
#include <sstream>
#include <fstream>

#include "matrix_manip_cleanup.h"

using namespace std;

// ---------------- Print ---------------- //
/*
template<class T>
void qbMatrix2<T>::PrintMatrix() {
    for (int i = 0; i < m_nRows; i++) {
        for (int j = 0; j < m_nCols; j++)
            std::cout << std::setw(10) << m_matrix_Data[Sub2Ind(i, j)] << " ";
        std::cout << "\n";
    }
}
*/
/*
int main() {
    std::string filename = "test_matrices.csv";

    /*Openfile*/
    ifstream input(filename);
    if(input.is_open()){

        //the file has one header line containing the number of rows and columns of each matrix
        std::string currline;
        getline(input, currline);

        // Accept both "# n" and "n" as header
        int nRows = 0;
        size_t spacepos = currline.find(" ");
        if (spacepos != std::string::npos) {
            std::string nrows_str = currline.substr(spacepos, currline.length());
            nRows = atoi(nrows_str.c_str());
        } else {
            nRows = atoi(currline.c_str());
        }

//Read the rest of the file and parse each line as we go.
std::stringstream ss;
double currentNumber;

/*currentRow will hold all the elements that make up the row we have just read, which will correspond to a single line form the .CSV file*/
std::vector<double> currentRow;
/*currentRow will hold all the data from the elements that make up the row we have just read,
which will correspond to the single line from the CSV file. */

std::vector<double> currentMatrixData;

/*Create a vector of qbMatrix2 pointers so that we acan hold the matrices that resuly in any failed tests */
std::vector<qbMatrix2<double>*> failedMatrices;

int totalRows = 0;
int rowCount = 0;
int numSuccess = 0;
int numFailures = 0;
//loop untill we reach the EOF

while (!input.eof()){
    getline(input, currline);
    
    //convert to string stream object   
    ss<< currline;
    //loop through the string stream to extract the numbers around the commas
    while(ss >> currentNumber){
        currentRow.push_back(currentNumber);
        if(ss.peek() == ','){
            ss.ignore();
        }
    }

    //Clear the string stram ready for the next loop, since we don't create a new one each time
    // (currentRow now contains all the numbers we need)
    ss.clear();

    /*concatinate the elsments of the current row into the currentmatrix, which will be one large and single dimentional array*/
    currentMatrixData.insert(currentMatrixData.end(), currentRow.begin(), currentRow.end());

    //Clear the current row ready for the next loop
    currentRow.clear();

    //Incerement the row count
    rowCount++;

    /*if we have read 5 rows, then w stop and form this data into a qbMatrix2 object with dimentions of 5 rows and 10 cols*/
    if(rowCount > 4){
        //Reset the row count
        rowCount = 0;
        //create the qbMatrix2 object object and store the matrix data
        qbMatrix2<double> newMatrix(5,10,&currentMatrixData);

        //prepare the two matrices to contatin the left and the right halves of this matrix
        // (each of these will be square, with dimentions of 5x5)
        qbMatrix2<double> leftMatrix;
        qbMatrix2<double> rightMatrix;
        //seperate the new matrix into its left and right halves
        newMatrix.Seperate(&leftMatrix, &rightMatrix, 5);

        /*the right hand side now contains the golden result (inverse) loaded from the CSV file, while the left hand side is original matrix, Therefore we want to invert the 
        left hand side ourselfs and then compare the result with teh right hand side*/

        leftMatrix.Inverse();

        /*note the use of the 'compare' function with a specified tolerance of 1e-9.*/
        if(leftMatrix.Compare(rightMatrix, 1e-9)){
            numSuccess++;
            //std::cout << "Test " << totalRows/5 << " passed" << std::endl;
            std::cout << "Test passed" << std::endl;
        }
        else{
            numFailures++;
            //std::cout << "Test " << totalRows/5 << " failed" << std::endl;
            std::cout << "Test failed" << std::endl;
            //store a pointer to the failed matrix
            failedMatrices.push_back(new qbMatrix2<double>(newMatrix));
        }

        currentMatrixData.clear();

    }
    totalRows++;

}

cout<<"Num success = " << numSuccess << std::endl;
cout<<"Num failures = " << numFailures << std::endl;

//print out any failed matrices
for(qbMatrix2<double>* currentMatrix : failedMatrices){
    currentMatrix->PrintMatrix();
}

input.close();
    return 0;
}
}
*/ */
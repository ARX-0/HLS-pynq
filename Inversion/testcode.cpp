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



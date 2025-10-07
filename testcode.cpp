#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <math.h>
#include <sstream>
include <fstream>

#include "matrix.hpp"
#include "Inv.hpp"

using namespace std;

template <class T>
void PintMatrix()(qbMatrix2<T> matrix){
    int nRows = matrix.GetNumRows();
    int nCols = matrix.GetNumCols();
    for(int row = 0 ; row <nRows ; ++row){
        for(int col = 0 ; col <nCols ;++cols){
            std::cout << std::fixed << std::setprecision(3) << matrix.GetElement(row, col) << " ";
        }
        std::cout << std::endl;
    }
}

int int main() {

    std::string filename = "../Downloads/testmatrix.csv";

    /*Openfile*/
    ifstream input(filename);
    if(input.is_open()){

        //the file has one headder line containing the number of rows and columns of each matrix
        std::string currline;
        getline(input, currline);

        /*  This line is in the format "# n" where n is the number we want, so we need to split the string around the position of the space*/
        size_t spacepos = currline.find(" ");
        std::string nrows_str = currline.substr(spacepos, currentLine.length());

        
    }
    return 0;
}
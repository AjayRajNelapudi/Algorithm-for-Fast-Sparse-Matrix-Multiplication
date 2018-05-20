//
//  main.cpp
//  Sparse Matrix
//
//  Created by Ajay Raj Nelapudi on 17/05/18.
//  Copyright © 2018 Ajay Raj Nelapudi. All rights reserved.
//

#include <iostream>
#include <ctime>
#include "sparse.h"
using namespace std;

int a[r1][c1];
int b[r2][c2];
int res[r1][c2];

void multiply() {
    for (int i=0; i<r1; i++) {
        for (int j=0; j<c2; j++) {
            for (int k=0; k<r2; k++) {
                res[i][j]+= a[i][k] * b[k][j];
            }
        }
    }
}

int main() {
    clock_t start, stop;
    int **product;

    const int limit = 1000;
    const int step = 10;
    
    for (int i=0; i<r1; i++) {
        for (int j=0; j<c1; j+=step) {
            a[i][j] = rand() % limit + 1;
        }
    }
    
    for (int i=0; i<r2; i++) {
        for (int j=0; j<c2; j+=step) {
            b[i][j] = rand() % limit + 1;
        }
    }
    
    SparseMatrix matrix(r1, c1, r2, c2);
    matrix.formatTriplets(a, b);
    start = clock();
    product = matrix.multiply();
    stop = clock();
    double newTime = (double)(stop - start) / CLOCKS_PER_SEC;
    
    
    start = clock();
    multiply();
    stop = clock();
    double exisTime = (double)(stop - start) / CLOCKS_PER_SEC;
    
    for (int i=0; i<r1; i++) {
        for (int j=0; j<c2; j++) {
            if (product[i][j] != res[i][j]) {
                cout << "Incorrect" << endl;
                exit(0);
            }
        }
    }
    
    /*for (int i=0; i<r1; i++) {
        for (int j=0; j<c2; j++) {
            cout << product[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl << endl << endl;
    
    for (int i=0; i<r1; i++) {
        for (int j=0; j<c2; j++) {
            cout << res[i][j] << " ";
        }
        cout << endl;
    }*/
    
    cout << "New Algorithm run-time: " << newTime << " s" << endl;
    cout << "Naive Algorithm run-time: " << exisTime << " s" << endl;
    
    double sparsityA = 1 - (double)matrix.tripletA[0][2] / (r1 * c1);
    double sparsityB = 1 - (double)matrix.tripletB[0][2] / (r2 * c2);
    cout << "Sparsity of A = " << sparsityA * 100 << " %" << endl;
    cout << "Sparsity of B = " << sparsityB * 100 << " %" << endl;
    
    cout << newTime << "," << exisTime << endl;
    
    return 0;
}

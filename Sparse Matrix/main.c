//
//  main.c
//  Sparse Matrix
//
//  Created by Ajay Raj Nelapudi on 16/08/18.
//  Copyright © 2018 Ajay Raj Nelapudi. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int *hashMap;

void print(int rows, int columns, int a[][columns]) {
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < columns; j++) {
            printf("%d ", a[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int** matrixToTriplet(int rows, int columns, int matrix[rows][columns], int hashMapEnable) {
    int nonZeroElements = 0;
    int **triplet = (int **) malloc(sizeof(int *) * (rows * columns + 1));
    for(int i = 0; i < rows * columns + 1; i++) {
        triplet[i] = (int *) malloc(sizeof(int) * 3);
    }
    
    int currentRow = -1;
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < columns; j++) {
            if (matrix[i][j] == 0) continue;
            
            nonZeroElements++;
            
            if (currentRow != i && hashMapEnable) {
                hashMap[i] = nonZeroElements;
                currentRow = i;
            }
            
            triplet[nonZeroElements][0] = i;
            triplet[nonZeroElements][1] = j;
            triplet[nonZeroElements][2] = matrix[i][j];
        }
    }
    
    triplet[0][0] = rows;
    triplet[0][1] = columns;
    triplet[0][2] = nonZeroElements;
    
    return triplet;
}

int** multiply(int **tripletA, int **tripletB, int rows, int columns) {
    int ** product = (int **) malloc(sizeof(int *) * rows);
    for(int i = 0; i < rows; i++) {
        product[i] = (int *) malloc(sizeof(int) * columns);
    }
    
    for (int i=1; i<=tripletA[0][2]; i++) {
        int j = hashMap[tripletA[i][1]];
        
        if (j == -1) continue;
        
        while (tripletA[i][1] == tripletB[j][0] && j < rows * columns) {
            product[tripletA[i][0]][tripletB[j][1]] += tripletA[i][2] * tripletB[j][2];
            j++;
        }
    }
    
    return product;
}

int** naiveMatrixMultiplication(int rows, int columns, int matrixA[][columns], int matrixB[][columns]) {
    int **res = (int **) malloc(sizeof(int *) * rows);
    for(int i = 0; i < rows; i++) {
        res[i] = (int *) malloc(sizeof(int) * columns);
    }
    
    for (int i=0; i<rows; i++) {
        for (int j=0; j<columns; j++) {
            int sum = 0;
            for (int k=0; k<rows; k++) {
                sum += matrixA[i][k] * matrixB[k][j];
            }
            res[i][j] = sum;
        }
    }
    
    return res;
}

int main() {
    const int n = 1000;
    const int step = n/10;
    
    hashMap = (int *) malloc(sizeof(int) * n * n);
    for(int i = 0; i < n * n; i++) {
        hashMap[i] = -1;
    }
    
    int a[n][n];
    int b[n][n];
    
    for(int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if(j % step == 0) {
                a[i][j] = rand() % 1000 + 1;
                b[j][i] = rand() % 1000 + 1;
            } else {
                a[i][j] = 0;
                b[j][i] = 0;
            }
        }
    }
    
    /*print(n, n, a);
    print(n, n, b);*/
    
    clock_t start, stop;
    
    start = clock();
    int **tripletA = matrixToTriplet(n, n, a, 0);
    int **tripletB = matrixToTriplet(n, n, b, 1);
    int **result = multiply(tripletA, tripletB, n, n);
    stop = clock();
    double newTime = (double)(stop - start)/CLOCKS_PER_SEC;
    
    start = clock();
    int **naiveResult = naiveMatrixMultiplication(n, n, a, b);
    stop = clock();
    
    double naiveTime = (double)(stop - start)/CLOCKS_PER_SEC;
    /*print(n, n, result); // You might be a great developer but you'll still end up printing
    print(n, n, naiveResult); // Because the Human Brain works best after visualising*/
    
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if (result[i][j] != naiveResult[i][j]) {
                printf("Error\n");
                return 0;
            }
        }
    }
    
    printf("Correct\n");
    printf("New Time %lf\n", newTime);
    printf("Naive Time %lf\n", naiveTime);
    
    return 0;
}

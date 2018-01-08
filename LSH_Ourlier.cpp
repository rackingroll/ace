#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <cstring>
#include <chrono>
#include <cmath>
#include "SignedRandomProjection.h"
#include "LSH.h"
#include "LSH_Ourlier.h"

using namespace std;

bool *lsh(vector<double *> &data, int numData, int dim, double *time, int K, int L, double alpha) {
    //printf("LSH configuration: K = %d, L = %d, alpha = %f\n", K, L, alpha);

    auto start_time = chrono::high_resolution_clock::now();

    //Create HashTables
    LSH *_Algo = new LSH(K, L, K);
    SignedRandomProjection *proj = new SignedRandomProjection(dim, K * L);

    for (int i = 0; i < numData; i++) {
        int *hashes = proj->getHash(data[i], dim);
        _Algo->add(hashes, i);
    }

    double mean = 0;
    double stdev = 0;

    double *localmean = new double[numData];
#pragma omp parallel for
    for (int i = 0; i < numData; i++) {
        int *hashes = proj->getHash(data[i], dim);
        localmean[i] = _Algo->retrieve_mean(hashes);
    }

    for (size_t i = 0; i < numData; i++) {
        mean += localmean[i];
    }
    mean = mean / (double) numData;
    for (int i = 0; i < numData; i++) {
        stdev += (localmean[i] - mean) * (localmean[i] - mean);
    }
    stdev = sqrt(stdev / (numData - 1));

    double threshold = mean - alpha * stdev;
    //printf("numData = %d, mean = %f, stdev = %f, threshold = %f\n", numData, mean, stdev, threshold);

    bool *outlier = new bool[numData];

#pragma omp parallel for
    for (int i = 0; i < numData; i++) {
        outlier[i] = localmean[i] < threshold;
    }

    delete _Algo;
    delete proj;
    delete[]localmean;
    *time = (chrono::high_resolution_clock::now() - start_time).count() * 1e-9;

    return outlier;
}

std::vector<double *> load(string filename, int dimension) {
    ifstream file(filename);
    string str;
    std::vector<double *> data;
    while (getline(file, str)) {
        double *row = new double[dimension];
        int i = 0;
        std::stringstream stream(str);
        double d;
        while (stream >> d) {
            row[i++] = d;
        }
        if (i != dimension) {
            cout << "ERROR: dimension doesn't match." << endl;
            exit(1);
        }
        data.push_back(row);
    }
    return data;
}

#include <iostream>
#include "LSH.h"
/* Author: Chen Luo and Anshumali Shrivastava
*  COPYRIGHT PROTECTION
*  Free for research use.
*  For commercial use, contact:  RICE UNIVERSITY INVENTION & PATENT or the Author.
*/

using namespace std;

LSH::LSH(int K, int L, int rangebits) {
    _K = K;
    _L = L;
    _rangebits = rangebits;
    _bucket = new int[_L * (1 << _rangebits)]();
    int totsize = _L * (1 << _rangebits);

}

void LSH::add(int *hashes, int id) {
    int index = 0;

#pragma omp parallel for
    for (int i = 0; i < _L; i++) {
        int index = 0;
        for (int j = 0; j < _K; j++) {
            index = (index << 1) + hashes[_K * i + j];
        }
        _bucket[i * (1 << _rangebits) + index] = _bucket[i * (1 << _rangebits) + index] + 1;
    }
}

int *LSH::retrieve(int *hashes) {
    int *retreived = new int[_L];
    //std::unordered_map<int, int> m;

    int count = 0;

#pragma omp parallel for
    for (int i = 0; i < _L; i++) {
        int index = 0;
        for (int j = 0; j < _K; j++) {
            index = (index << 1) + hashes[_K * i + j];
        }
        retreived[i] = _bucket[i * (1 << _rangebits) + index];
    }

    return retreived;
}

double LSH::retrieve_mean(int *hashes) {
    int sum = 0;
    int *retrivd = retrieve(hashes);
    for (int i = 0; i < _L; i++) {
        sum += retrivd[i];
    }

    return sum / (double) _L;
}

LSH::~LSH() {
    delete[]_bucket;
}

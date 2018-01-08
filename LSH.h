#pragma once

class LSH {
private:
    int *_bucket;
    int _K;
    int _L;
    int _rangebits;
public:
    LSH(int K, int L, int rangebits);
    void add(int *hashes, int id);
    int *retrieve(int *hashes);
    double retrieve_mean(int *hashes);
    virtual ~LSH();
}; 

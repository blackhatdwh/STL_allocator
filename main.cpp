#include <iostream>
#include <ctime>
#include "mempool.h"
#define MAX 1000000
using namespace std;
int main(){
    MemoryPool<float> pool;

    std::clock_t start;
    double duration;
    start = std::clock();

    float *x[MAX];
    for(int i =0 ; i < MAX; i++){
        x[i] = pool.allocate();
        *x[i] = 123;
    }

    for(int i =0 ; i < MAX; i++){
        pool.deallocate(x[i]);
    }


    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    cout << duration << endl;
}

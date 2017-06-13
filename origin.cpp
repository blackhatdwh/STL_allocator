#include <iostream>
#include <ctime>
#include "mempool.h"
#define MAX 1000000
using namespace std;
int main(){
    std::clock_t start;
    double duration;
    start = std::clock();
    float* x[MAX];
    for(int i =0 ; i < MAX; i++){
        x[i] = new float;
        *x[i] = 123;
    }

    for(int i =0 ; i < MAX; i++){
        delete x[i];
    }
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

    cout << duration << endl;
}

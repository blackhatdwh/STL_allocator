#include <vector>
#include <iostream>
#include "mempool.h"

using namespace std;

int main(){
    vector<int, MemoryPool<int> > a;
    for(int i=0;i<4000;i++){
        a.push_back(i);
    }
}

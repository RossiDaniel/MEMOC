#include <cstdio>
#include <vector>
#include <deque>
#include <iostream>
#include <random>
#include <algorithm>


/*
    esempio s = 1 sz = 10 rz = 5
    vettore [0,0,0,0,0,0,0,0,0,0]
    iota -> [1,2,3,4,5,6,7,8,9,10]
    resize -> [1,2,3,4,5]
*/
std::vector<int> uniform_shuffle(int s, int sz, int rz){
    std::vector<int> v(sz,0);
    std::iota(v.begin(), v.end(), s);
    std::random_shuffle (v.begin(), v.end());
    v.resize(rz);
    return v;
}
#include <vector>
#include <iostream>
#include <cstdio>
#include "parse.cpp"

using namespace std;

int main (int argc, char const *argv[]){
    vector<int> S = {0,27,5,11,8,4,25,28,2,1,19,9,3,14,17,16,13,21,10,18,24,6,22,26,7,23,15,12,20,0};
    vector<int> S = {0 20 12 15 23 7 26 22 6 24 18 10 21 13 16 17 14 3 9 19 1 2 28 25 4 8 11 5 27 0}
	//vector<int> S = {0,7,37,30,43,17,6,27,5,36,18,26,16,42,29,35,45,32,19,46,20,31,38,47,4,41,23,9,44,34,3,25,1,28,33,40,15,21,2,22,13,24,12,10,11,14,39,8,0};
    //vector<int> S = {0,7,37,30,43,17,6,27,5,36,18,26,16,42,29,35,45,32,19,46,20,31,38,47,4,41,23,9,44,34,3,25,1,28,33,40,15,21,2,22,13,24,12,10,11,14,39,8,0};
    vector<vector<double>> C = parse("data/data29.csv");
    int obj = 0;
    /*
    for(int i=0; i<S.size(); i++){
        S[i] --;
        cout<<S[i]<<",";
    }
    
    for(int i=0; i < C.size(); i++){
        for(int j=0; j < C[i].size();j++){
            cout<<C[i][j]<<" ";
        }
        cout<<endl; 
    }
    */
    for(int i=0; i< S.size()-1;i++){
        obj += C[S[i]][S[i+1]];
    }
    cout<<obj;
    
}
#include <cstdio>
#include <vector>
#include <deque>
#include <iostream>
#include <map>
#include "genetic.cpp"
#include "../data/parse.cpp"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <chrono>
#include <ctime>

using namespace std;
bool comparechromosome(chromosome* a, chromosome* b) { return (a->fit() > b->fit()); }

void genetic(std::string n, int p,int s, int l, int k, int m, int f, int t){
    std::string file_path= "../data/data"+n+".csv";
	vector<vector<double>*> C = parse(file_path);
    
    //generate random initial population
    vector<chromosome*> P = initialpopulation(p,C);
    chromosome * best = P[0];
    
    int x = 0;
    double ct = 0;
    std::chrono::_V2::system_clock::time_point start,end;

    while(x < f && ct < t){
        start = std::chrono::system_clock::now();
        
        LS2opt(P,l,k);

        std::sort(P.begin(), P.end(),comparechromosome);       

        if(best->fit() > P[P.size()-1]->fit()){
            best = new chromosome(P[P.size()-1]);
            best->print();
            x=0;
        }
        else{
            x++;
        }

        std::vector<chromosome*> selected = get_discrete_distribution(P,s);
        crossover(selected);

        P = selected;
        mutation(P,m);
        end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;
        ct = ct + elapsed_seconds.count();
    }

    for(int i=0;i<P.size();i++){
        delete P[i];
    }
    std::cout<<best->fit()<<" "<<ct<<std::endl;
}

int main (int argc, char const *argv[]){
    std::string n= argv[1]; // numero istanza
    int p = strtod(argv[2], NULL); // quantita' popolazione 
    int s = strtod(argv[3], NULL); // numero popolazione selezionata
    int l = strtod(argv[4], NULL); // numero popolazione local search
    int k = strtod(argv[5], NULL); // numero città da swappare
    int m = strtod(argv[5], NULL); // numero popolazione mutazione
    int f = strtod(argv[7], NULL); // iterazione fallimentari
    int t = strtod(argv[8], NULL); // time limit secondi

    genetic(n,p,s,l,k,m,f,t);
}
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
/*
inline bool exist(const std::string& name)
{
    ifstream file(name);
    if(!file)            // If the file was not found, then file is 0, i.e. !file=1 or true.
        return false;    // The file was not found.
    else                 // If the file was found, then file is non-0.
        return true;     // The file was found.
}
*/
int main (int argc, char const *argv[]){
	/*
	if((argc == 0) || (argc > 0 && !exist(argv[1]))){
		cout<<"dat file not found"<<endl;
		return 0;
	}
    srand (time(NULL));
    */
   
    //create vector of costs
    std::string inst = argv[1];
    std::string file_path= "../data/data"+inst+".csv";
	vector<vector<double>*> C = parse(file_path);

    int n = strtod(argv[2], NULL); // quantita' popolazione 
    int s = strtod(argv[3], NULL); // numero popolazione selezionata
    int l = strtod(argv[4], NULL); // numero popolazione local search
    int k = strtod(argv[5], NULL); // numero città da swappare
    int m = strtod(argv[5], NULL); // numero popolazione mutazione
    int f = strtod(argv[7], NULL); // iterazione fallimentari
    int t = strtod(argv[8], NULL); // time limit secondi

    //generate random initial population
    vector<chromosome*> P = initialpopulation(n,C);
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
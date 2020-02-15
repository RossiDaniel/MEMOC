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

inline bool exist(const std::string& name)
{
    ifstream file(name);
    if(!file)            // If the file was not found, then file is 0, i.e. !file=1 or true.
        return false;    // The file was not found.
    else                 // If the file was found, then file is non-0.
        return true;     // The file was found.
}

int main (int argc, char const *argv[]){
	
	if((argc == 0) || (argc > 0 && !exist(argv[1]))){
		cout<<"dat file not found"<<endl;
		return 0;
	}
    srand (time(NULL));

    //create vector of costs
	vector<vector<double>*> C = parse(argv[1]);

    int N = strtod(argv[2], NULL); // quantita' popolazione 
    int S = strtod(argv[3], NULL); // numero popolazione selezionata
    int LS = strtod(argv[4], NULL); // numero popolazione local search
    int nswap = strtod(argv[5], NULL); // numero città da swappare
    int M = strtod(argv[5], NULL); // numero popolazione mutazione
    int it = strtod(argv[7], NULL); // iterazione fallimentari
    int TM = strtod(argv[8], NULL); // time limit secondi

    //generate random initial population
    vector<chromosome*> p = initialpopulation(N,C);
    chromosome * best = p[0];
    
    int n = 0;
    double current = 0;
    std::chrono::_V2::system_clock::time_point start,end;

    
    while(n < it && current < TM){
        start = std::chrono::system_clock::now();
        std::cout<<"1"<<std::endl;
        
        LS2opt(p,LS,nswap);
                std::cout<<"2"<<std::endl;

        std::sort(p.begin(), p.end(),comparechromosome);       
         std::cout<<"3"<<std::endl;

        //for(int i=0; i< p.size(); i++){p[i]->print();}
        std::cout<<best->fit() <<" "<< p[p.size()-1]->fit()<<std::endl;
        if(best->fit() > p[p.size()-1]->fit()){
            best = new chromosome(p[p.size()-1]);
            //best->print();
            n=0;
        }
        else{
            n++;
        }
         std::cout<<"4"<<std::endl;

        std::vector<chromosome*> selected = get_discrete_distribution(p,S);
                std::cout<<"5"<<std::endl;

        std::vector<chromosome*> children;
        for(int i=0; i<selected.size();i=i+2)
        {
            //std::cout<<selected[i]->print()<<" "<<selected[i+1]->print()<<std::endl;
            std::cout<<"-------"<<std::endl;
            selected[i]->print();
            selected[i+1]->print();
            crossovercx2(selected[i],selected[i+1],children);
        }
        std::cout<<"6"<<std::endl;

        selected.insert( selected.end(), children.begin(), children.end() );
        p = selected;
        mutation(p,M);
        end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;
        current = current + elapsed_seconds.count();
    }
    std::cout<<"best"<<std::endl;
    best->print();
}
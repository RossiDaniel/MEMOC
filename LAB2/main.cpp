#include <cstdio>
#include <vector>
#include <deque>
#include <iostream>
#include <map>
#include "genetic.cpp"
#include "../data/parse.cpp"

using namespace std;
bool comparechromosome(chromosome* a, chromosome* b) { return (*a < *b); }

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

    //create vector of costs
	const vector<vector<double>>* C = new vector<vector<double>>(parse(argv[1]));
    
    //generate random initial population
    vector<chromosome*> p = initialpopulation(10000,C);
    for(int i=0;i<p.size();i=i+100){
        p[i]->LS2opt();
    }

    //sort initial population
    
    for(int i=0; i<100;i++){
        std::sort(p.begin(), p.end(),comparechromosome);
        std::cout<<"best "<<i<<std::endl;
        p[p.size()-1]->print();
        std::cout<<std::endl;
        //create discrete distribution based on rank selection on pop/4


        std::vector<chromosome*> selected = get_discrete_distribution(p);
        std::vector<chromosome*> children;
        for(int i=0; i<selected.size()-1;i++)
        {
            //std::cout<<i<<std::endl;
            crossovercx2(selected[i],selected[i+1],children);
        }

        for(int i=0;i<children.size();i=i+50){
            children[i]->LS2opt();
        }

        selected.insert( selected.end(), children.begin(), children.end() );
        p = selected;
    }
}
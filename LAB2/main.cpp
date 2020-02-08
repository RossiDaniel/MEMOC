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
    
    int N = strtod(argv[2], NULL);
    int subp = strtod(argv[3], NULL);
    int LS = strtod(argv[4], NULL);
    int it = strtod(argv[5], NULL);

    //generate random initial population
    vector<chromosome*> p = initialpopulation(N,C);

    //sort initial population
    int i = 0;
    while(i < it){
        for(int i=0;i<(double(p.size())/100)*LS;i++){
            p[i]->LS2opt();
        }

        std::sort(p.begin(), p.end(),comparechromosome);
        std::cout<<"best "<<i<<" population: "<<p.size()<<std::endl;
        p[p.size()-1]->print();
        std::cout<<std::endl;

        std::vector<chromosome*> selected = get_discrete_distribution(p,subp);
        std::vector<chromosome*> children;
        for(int i=0; i<selected.size()-1;i++)
        {
            crossovercx2(selected[i],selected[i+1],children);
        }

        selected.insert( selected.end(), children.begin(), children.end() );
        p = selected;
        i++;
    }
}
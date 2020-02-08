#include "chromosome.h"
#include <vector>
#include <set>

chromosome::chromosome(std::vector<int> chro,const std::vector<std::vector<double>>* costvec): c(chro), cost(costvec){};

double chromosome::fit()const
{    
    if(find(-1) != -1){return 0;}

    double fitness = (*cost)[c[c.size()-1]][c[0]];
    for(int i=0; i< c.size()-1;i++){
            fitness += (*cost)[c[i]][c[i+1]];
        }
    
    return fitness;
}

bool chromosome::feasible()const
{
    std::set<int> s(c.begin(), c.end());
    return s.size() == c.size();
}

int chromosome::operator[](int i)const
{
    return c[i];
}

int chromosome::size()const
{
    return c.size();
}

bool chromosome::operator<(const chromosome& c)const{
    return fit() >= c.fit();
}

void chromosome::print()const{
    std::cout<<fit()<<" :";
    for(int j=0; j < c.size(); j++){
        std::cout<<c[j]<<" ";
    }

    std::cout<<". "<<feasible()<<std::endl;
}

int chromosome::find(int n)const{
    for(int i=0; i< c.size();i++)
    {
        if( c[i] == n){return i;} 
    }
    return -1;
}

const std::vector<std::vector<double>>* chromosome::getcost(){
    return cost;
}

void chromosome::LS2opt(){
    for(int i=1; i<c.size()-2;i++)
    {
        for(int j=i+1;j<c.size()-1;j++)
        {
            double fitness = fit();
            int Cnew = fitness - (*cost)[c[i-1]][c[i]] - (*cost)[c[j]][c[j+1]] + (*cost)[c[i-1]][c[j]] + (*cost)[c[i]][c[j+1]];
            //std::cout<<fitness<<" "<<Cnew<<std::endl;
            if(fitness > Cnew){
                int temp = c[i];
                c[i] = c[j];
                c[j] = temp;
            }
        }
    }
}

void chromosome::reverse(int a,int b){
    std::reverse(c.begin()+a,c.begin()+b);
}

int chromosome::nearestcity(int city){
    int nearestcity= 0;
    for(int i=0; i<cost->size();i++){
        
        if ((i != city) && (*cost)[nearestcity][city] > (*cost)[i][city]){
            nearestcity = i;
        }
    }
    return nearestcity;
}
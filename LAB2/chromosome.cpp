#include "chromosome.h"
#include <vector>
#include <set>
#include <iostream>
using namespace std;

chromosome::chromosome(std::vector<int> chro,const std::vector<std::vector<double>*> costvec): c(chro), cost(costvec){};

double chromosome::fit()const
{    
    if(find(-1) != -1){return 0;}

    double fitness = (*cost[c[c.size()-1]])[c[0]];
    for(int i=0; i< c.size()-1;i++){
            fitness += (*cost[c[i]])[c[i+1]];
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

const std::vector<std::vector<double>*> chromosome::getcost(){
    return cost;
}

void chromosome::LS2opt(int ns){
    std::default_random_engine device;
    std::vector<int> v(c.size()-2);

    std::iota(v.begin(), v.end(), 1);

    std::uniform_int_distribution<> distr(1,v.size());
    std::vector<int> s;
    int number = 0;
    int n = 0;
    while(n < ns)
    {
        number = distr(device);
        s.push_back(number);
        n++;
    }
    std::sort(s.begin(),s.end());

    double fitness = fit();
    for(int i=0; i<s.size();i++)
    {
        for(int j=i+1; j<s.size();j++)
        {
            //individuano le posizione da reversare
            int x = s[i];
            int y = s[j];

            if(x == y){continue;}

            //salviamo il valore prima e quello dopo
            int ix = c[x-1];
            int yi = c[y+1];
            
            //salviamo i valori di x e y
            int XX = c[x];
            int YY = c[y];
            
            int Cnew = fitness - (*cost[ix])[XX] - (*cost[YY])[yi] + (*cost[ix])[YY] + (*cost[XX])[yi];
            //std::cout<<fitness<<" "<<Cnew<<std::endl;
            if(fitness > Cnew){
                reverse(x,y);
                fitness = fit();
            }
        }
    }
}

void chromosome::reverse(int a,int b){
    while(a < b){
        int temp = c[a];
        c[a] = c[b];
        c[b] = temp;
        a++;
        b--;
    }
}

int chromosome::nearestcity(int city){
    int nearestcity= 0;
    for(int i=0; i<cost.size();i++){
        
        if ((i != city) && (*cost[nearestcity])[city] > (*cost[i])[city]){
            nearestcity = i;
        }
    }
    return nearestcity;
}


#include "chromosome.h"
#include <vector>
#include <set>
#include <iostream>
#include "utility.cpp"

using namespace std;

chromosome::chromosome(std::vector<int> chro,const std::vector<std::vector<double>*> costvec): c(chro), cost(costvec){};
chromosome::chromosome(const chromosome* ch){
    c = ch->c;
    cost = ch->cost;
}

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

    std::cout<<". "<<feasible()<<std::endl<<std::endl;
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
    bool no_improvment = true;
    while (no_improvment){
        no_improvment = false;

        std::vector<int> s = uniform_shuffle(1,c.size()-2,ns);
        double fitness = fit();
        
        for(int i=0; i<s.size() && !no_improvment;i++)
        {
            for(int j=i+1; j<s.size() && !no_improvment;j++)
            {
                //individuano le posizione da reversare
                int x = std::min(s[i],s[j]);
                int y = std::max(s[i],s[j]);

                //salviamo il valore prima e quello dopo
                int ix = c[x-1];
                int yi = c[y+1];
                
                //salviamo i valori di x e y
                int XX = c[x];
                int YY = c[y];
                
                int Cnew = fitness - (*cost[ix])[XX] - (*cost[YY])[yi] + (*cost[ix])[YY] + (*cost[XX])[yi];
                if(fitness > Cnew){
                    reverse(x,y);
                    fitness = fit();
                    no_improvment = true;
                }
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
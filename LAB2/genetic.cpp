#include <cstdio>
#include <vector>
#include <deque>
#include <iostream>
#include <random>
#include <algorithm>
#include "chromosome.cpp"
#include <thread>

std::vector<chromosome*> initialpopulation(int nip, const std::vector<std::vector<double>*> cost){
    std::vector<chromosome*> p;

    for(int i=0; i< nip; i++)
    {
        std::vector<int> v(cost.size());
        std::iota(v.begin(), v.end(), 0);
        std::random_shuffle ( v.begin(), v.end());
        chromosome* c = new chromosome(v,cost);
        p.push_back(c);
    }
    return p;
}

void LS2opt(std::vector<chromosome*> cs,int ns, int param){
    std::vector<int> v = uniform_shuffle(0,cs.size()-1,ns);

    for(int i= 0; i< v.size(); i++)
    {
        cs[v[i]]->LS2opt(param);
    }
} 

std::vector<chromosome*> get_discrete_distribution(std::vector<chromosome*> cs,int ns){
    std::vector<double> v(cs.size());
    std::iota(v.begin(), v.end(), 1);
    for(int i=0;i<v.size();i++){
        v[i] = (v[i]*2)/(v.size()*(v.size()+1));
    }

    std::discrete_distribution< > distr(v.begin(),v.end());
    std::default_random_engine device(std::random_device{}());

    std::vector<chromosome*> nc;
    int number,n = 0;
    while(n < ns)
    {
        number = distr(device);
        nc.push_back(cs[number]);
        n++;
    }
    return nc;
}

void crossovercx2(chromosome* p1,chromosome* p2, std::vector<chromosome*>& cs){
    int index1 = 0;
    int index2 = 0;

    std::vector<int> child1(p1->size(),-1);
    std::vector<int> child2(p1->size(),-1);
    bool ok = true;
    int end = 0;
    int location = 0;

    while(ok)
    {
        child1[index1] = (*p2)[location];

        location = p1->find(child1[index1]);
        int value = (*p2)[location];
        location = p1->find(value);
        child2[index2] = (*p2)[location];

        if(child2[index2] == (*p1)[end])
        {
            bool exit = false;
            for(int i=0; i<p2->size() && !exit;i++){
                if (std::find(child1.begin(), child1.end(), (*p2)[i]) == child1.end())
                {
                    location = i;
                    end = i;
                    exit = true;
                    index1++;
                    index2++;
                }
            }
            if(exit == false){
                ok = false;
            }
        }
        else
        {
            location = p1->find(child2[index2]);
            index1++;
            index2++;
        }
    }
    chromosome* c1 = new chromosome(child1,p1->getcost());
    chromosome* c2 = new chromosome(child2,p2->getcost());
    cs.push_back(c1);
    cs.push_back(c2);
}

void mutation(std::vector<chromosome*> cs,int ns){
    std::vector<int> v = uniform_shuffle(0,cs.size(),ns);
    for(int i= 0; i< v.size(); i++){
        //invertion mutation
        int v1 = (rand() % (cs[i]->size()-1))+1;
        int v2 = (rand() % (cs[i]->size()-1))+1;
        if (v1 > v2){
            int temp = v1;
            v1 = v2;
            v2 = temp;
        }
        cs[v[i]]->reverse(v1,v2);
    }
}

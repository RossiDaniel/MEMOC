#include <cstdio>
#include <vector>
#include <deque>
#include <iostream>
#include <random>
#include <algorithm>
#include "chromosome.cpp"

std::vector<chromosome*> initialpopulation(int nip, const std::vector<std::vector<double>>* cost){
    std::vector<chromosome*> p;

    for(int i=0; i< nip; i++)
    {
        std::vector<int> v(cost->size());
        std::iota(v.begin(), v.end(), 0);
        std::random_shuffle ( v.begin(), v.end());
        
        chromosome* c = new chromosome(v,cost);
        p.push_back(c);
    }
    return p;
}

std::vector<chromosome*> get_discrete_distribution(std::vector<chromosome*> cs){
    std::vector<double> v(cs.size());
    std::iota(v.begin(), v.end(), 1);
    for(int i=0;i<v.size();i++){
        v[i] = (v[i]*2)/(v.size()*(v.size()+1));
    }

    std::discrete_distribution< > distr(v.begin(),v.end());

    std::random_device device;
    std::mt19937 generator(device());



    std::vector<int> selected(cs.size(),0);
    std::vector<chromosome*> nc;
    int newpop = 0;
    int number = 0;
    while(newpop < 1000)
    {
        number = distr(generator);
        if( selected[number] != 1)
        {
            selected[number]=1;
            nc.push_back(cs[number]);
            newpop++;
        }
    }
    /*
    for(int i=0;i<nc.size();i++){
        nc[i]->print();
    }
    */
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
        /*
        p1->print();
        p2->print();
        for(int i=0;i<child1.size();i++){
            std::cout<<child1[i]<<" ";
        }
        std::cout<<std::endl;
        for(int i=0;i<child2.size();i++){
            std::cout<<child2[i]<<" ";
        }
        std::cout<<std::endl;

        std::cout<<"----------"<<std::endl;
        */
        //insert p2[location] in child1 in position index1 
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


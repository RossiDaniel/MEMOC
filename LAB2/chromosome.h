#include <iostream>
#include <vector>
#include <random>

class chromosome{
    private:
        std::vector<int> c;
        std::vector<std::vector<double>*> cost;
    public:
        chromosome(std::vector<int>,const std::vector<std::vector<double>*>);
        chromosome(const chromosome*);
        double fit()const;
        bool feasible()const;
        int size()const;
        int operator[](int)const;
        bool operator<(const chromosome&)const;
        void print()const;
        int find(int n)const;
        const std::vector<std::vector<double>*> getcost();
        void LS2opt(int ns);
        void reverse(int,int);
        int nearestcity(int);
};
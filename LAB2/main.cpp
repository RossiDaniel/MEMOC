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
//funzione accessoria per funzione sort chromosome
bool comparechromosome(chromosome* a, chromosome* b) { return (a->fit() > b->fit()); }

void genetic(std::string n, int p,int s, int l, int k, int m, int f, int t){
    //creo matrice dei costi
    std::string file_path= "../data/data"+n+".csv";
	vector<vector<double>*> C = parse(file_path);
    
    //generazione di p individui random
    vector<chromosome*> P = initialpopulation(p,C);
    
    //inizializzo il chromosoma best
    chromosome * best = P[0];
    
    //numero di iterazioni fallimentari dall'ultima migliorativa
    int x = 0;
    //tempo trascorso dall'inizio dell'algoritmo
    double ct = 0;

    std::chrono::_V2::system_clock::time_point start,end;

    while(x < f && ct < t){
        start = std::chrono::system_clock::now();
        //eseguo la Local Search su l individui scambiando k^2 città
        LS2opt(P,l,k);
        //ordino la popolazione
        std::sort(P.begin(), P.end(),comparechromosome);       
        
        if(best->fit() > P[P.size()-1]->fit()){
            //salvo l'individuo migliore e azzero le iterazioni fallimentari
            best = new chromosome(P[P.size()-1]);
            x=0;
        }
        else{
            //iterazione fallimentare
            x++;
        }
        //seleziono la popolazione migliore
        std::vector<chromosome*> selected = select_population(P,s);

        //eseguo il crossover tra la popolazione selezionata
        crossover(selected);
        //eseguo la mutazione casuale su m individui
        mutation(selected,m);
        //nuova popolazione iniziale
        P = selected;
        //conteggi vari per il tempo
        end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;
        ct = ct + elapsed_seconds.count();
    }
    //dealloco ultima popolazione
    for(int i=0;i<P.size();i++){
        delete P[i];
    }
    std::cout<<"tempo: "<<ct<<std::endl;
    best->print();
    delete best;
}

int main (int argc, char const *argv[]){
    std::string n= argv[1]; // numero istanza
    int p = strtod(argv[2], NULL); // quantita' popolazione 
    int s = strtod(argv[3], NULL); // numero popolazione selezionata
    int l = strtod(argv[4], NULL); // numero popolazione local search
    int k = strtod(argv[5], NULL); // numero città da swappare
    int m = strtod(argv[5], NULL); // numero popolazione mutazione
    int f = strtod(argv[7], NULL); // iterazione fallimentari
    int t = strtod(argv[8], NULL); // time limit secondi

    genetic(n,p,s,l,k,m,f,t);
}
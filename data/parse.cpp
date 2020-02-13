#include <cstdio>
#include <iostream>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

using namespace std;

vector<vector<double>*> parse(string f){
    string line;
    ifstream myfile (f);

    vector<vector<double>*> tokens;


    if (myfile.is_open())
    {    
        while ( getline (myfile,line))
        {
            istringstream iss(line);
            string token;
            vector<double>* local = new vector<double>;
            while(getline(iss, token, ' '))
            {
                            //std::cout<<token<<std::endl;
                if(token != ""){
                    double value = strtod(token.c_str(), NULL);
                    local->push_back(value);
                }
            }
            tokens.push_back(local);
        }
        myfile.close();
    }
    else
    {
        cout << "Unable to open file"; 
    }

    return tokens;
}
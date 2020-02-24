#include <cstdio>
#include <vector>
#include <deque>
#include <iostream>
#include "../data/parse.cpp"
#include "cpxmacro.h"
#include <chrono>
#include <ctime>
using namespace std;	

// error status and messagge buffer
int status;
char errmsg[BUF_SIZE];

vector<vector<int>> map_x;
vector<vector<int>> map_y;

const int NAME_SIZE = 512;
char name[NAME_SIZE];

int N = 0;
const int O = 0;
const double cost0 = 0.0;
const double cost1 = 1.0;

void setupLP(CEnv env, Prob lp, vector<vector<double>*>& C, int & NumVar){
    //	MAP FOR X VARS: allocate map
	map_x.resize(N);
	for ( int i = 0 ; i < N ; ++i ) {
		map_x[i].resize(N);
		for ( int j = 0 ; j < N ; ++j ) {
			map_x[i][j]= -1;
		}
	} 

    //	MAP FOR Y VARS: allocate map
    map_y.resize(N);
	for ( int i = 0 ; i < N ; ++i )
	{
		map_y[i].resize(N);
		for ( int j = 0 ; j < N ; ++j )
		{
			map_y[i][j] = -1;
		}
	}

	//Add Var X
	const int x_init = 0; // first index for x vars
	int xcount = x_init;

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if(i != j && j != O)
			{
				char xtype = 'C';
				double obj = 0.0;
				double lb = 0.0;
				double ub = CPX_INFBOUND;
				snprintf(name, NAME_SIZE, "x_%d_%d", i, j);
				char* xname = (char*)(&name[0]);
				CHECKED_CPX_CALL( CPXnewcols, env, lp, 1, &obj, &lb, &ub, &xtype, &xname );
				
				//assegno il valore corrispondente alla map_x e aggiorno il count
				map_x[i][j] = x_init + xcount;
				xcount = xcount +1;
				NumVar = NumVar +1;
			}
		}
	}


	//Add Var Y
	const int y_init = x_init + xcount; // first index for y vars
	int ycount = 0;

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if(i != j)
			{
				char ytype = 'B';
				double obj = (*C[i])[j];
				double lb = 0.0;
				double ub = 1.0;
				snprintf(name, NAME_SIZE, "y_%d_%d", i, j);
				char* yname = (char*)(&name[0]);
				CHECKED_CPX_CALL( CPXnewcols, env, lp, 1, &obj, &lb, &ub, &ytype, &yname );
				
				//assegno il valore corrispondente alla map_x e aggiorno il count
				map_y[i][j] = y_init + ycount;
				ycount = ycount +1;
				NumVar = NumVar +1;
			}
		}
	}

	// add capacity constraints (origin) [ forall k -{O}, sum{i:(i,k) } x_ik - sum{j:(k,j) j != O} x_kj  = 1 ]
	for (int k = 0; k < N; k++)
	{
		if (k == O){continue;}
		std::vector<int> idx;
		std::vector<double> coef;
		
		for (int i = 0; i < N; i++)
		{
			if(i == k){continue;}
			idx.push_back(map_x[i][k]); // corresponds to variable x_ik
			coef.push_back(1.0);
		}

		for (int j = 0; j < N; j++)
		{
			if (j == O){continue;}
			if (k == j){continue;}

			idx.push_back(map_x[k][j]); // corresponds to variable x_kj
			coef.push_back(-1.0);
		}

		char sense = 'E';
		int matbeg = 0;
		CHECKED_CPX_CALL( CPXaddrows, env, lp, 0, 1, idx.size(), &cost1, &sense, &matbeg, &idx[0], &coef[0], 0, 0 );
	}

	// add capacity constraints (origin) [ forall i, sum{j:(i,j) } x_ij = 1 ]
	for (int i = 0; i < N; i++)
	{
		std::vector<int> idy;
		std::vector<double> coef;
		
		for (int j = 0; j < N; j++)
		{
			if(i == j){continue;}
			idy.push_back(map_y[i][j]); // corresponds to variable x_ij
			coef.push_back(1.0);
		}

		char sense = 'E';
		int matbeg = 0;
		CHECKED_CPX_CALL( CPXaddrows, env, lp, 0, 1, idy.size(), &cost1, &sense, &matbeg, &idy[0], &coef[0], 0, 0 );
	}

	// add capacity constraints (origin) [ forall j, sum{i:(i,j) } x_ij = 1 ]
	for (int j = 0; j < N; j++)
	{
		std::vector<int> idy;
		std::vector<double> coef;
		
		for (int i = 0; i < N; i++)
		{
			if(i == j){continue;}
			idy.push_back(map_y[i][j]); // corresponds to variable x_ij
			coef.push_back(1.0);
		}

		char sense = 'E';
		int matbeg = 0;
		CHECKED_CPX_CALL( CPXaddrows, env, lp, 0, 1, idy.size(), &cost1, &sense, &matbeg, &idy[0], &coef[0], 0, 0 );
	}

	// add capacity constraints (origin) [ forall (i,j) && j != O, x_ij - (N -1) * yij <= 0]
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if(i != j && j != O)
			{
				std::vector<int> id = {map_x[i][j],map_y[i][j]};
				std::vector<double> coef = {1.0,(-(N-1.0))};
			
				char sense = 'L';
				int matbeg = 0;
				CHECKED_CPX_CALL( CPXaddrows, env, lp, 0, 1, id.size(), &cost0, &sense, &matbeg, &id[0], &coef[0], 0, 0 );
			}
		}
	}

	CHECKED_CPX_CALL( CPXwriteprob, env, lp, "LAB1.lp", 0 );
}
//
inline bool exist(const std::string& name)
{
    ifstream file(name);
    if(!file)            // If the file was not found, then file is 0, i.e. !file=1 or true.
        return false;    // The file was not found.
    else                 // If the file was found, then file is non-0.
        return true;     // The file was found.
}

int main (int argc, char const *argv[]){
	std::string n = argv[1];
    std::string file_path= "../data/data"+n+".csv";
	vector<vector<double>*> C = parse(file_path);
    std::chrono::_V2::system_clock::time_point start,end;

	try
	{
		// init
		DECL_ENV( env );
		DECL_PROB( env, lp );
		// setup LP
		N = C.size();
		int NumVars = 0;
		setupLP(env, lp, C,NumVars);
		//timelimit
		CPXsetdblparam(env, CPX_PARAM_TILIM, strtod(argv[2], NULL));
		//time_elapsed
		double start,end;
  		CHECKED_CPX_CALL( CPXgettime, env, &start );
		//CPXgettime(env, &elapsed_time);

		// optimize
		CHECKED_CPX_CALL( CPXmipopt, env, lp );
		
		CHECKED_CPX_CALL( CPXgettime, env, &end );

		// print info
		double objval;
		CHECKED_CPX_CALL( CPXgetobjval, env, lp, &objval );

		std::cout<<"obj: "<<objval<<" "<<" elapsed_time: "<<end-start<<" Numvar: "<<NumVars<<std::endl;
		int n = CPXgetnumcols(env, lp);
		std::vector<double> varVals;
		varVals.resize(n);
		CHECKED_CPX_CALL( CPXgetx, env, lp, &varVals[0], 0, n - 1 );
		
		//print path
		std::deque<int> deq;
		deq.push_back(0);

		while(!deq.empty())
		{
			unsigned int c = deq.front();
    		deq.pop_front();
			for (unsigned int j = 0; j < map_y[c].size(); j++)
			{
				if(c != j && varVals[map_y[c][j]] == 1)
				{
					std::cout << c <<" ";
					if(j != 0){deq.push_back(j);}
					break;
				}
			}
		}
		
		CHECKED_CPX_CALL( CPXsolwrite, env, lp, "LAB1.sol" );
		// free
		CPXfreeprob(env, &lp);
		CPXcloseCPLEX(&env);
		
	}
	catch(std::exception& e)
	{
		std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
	}

	return 0;
}
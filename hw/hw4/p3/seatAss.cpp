#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include "sat.h"

using namespace std;

class Gate
{
public:
   Gate(unsigned i = 0): _gid(i) {}
   ~Gate() {}

   Var getVar() const { return _var; }
   void setVar(const Var& v) { _var = v; }

private:
   unsigned   _gid;  // for debugging purpose...
   Var        _var;
};

vector<Gate *> gates;


void
initCircuit(int n){
   for(int i = 0; i < n; ++i){
      gates.push_back(new Gate(i+1));
   }
}

void
genProofModel(SatSolver& s, int n){
   // Allocate and record variables; No Var ID for POs
   for (size_t i = 0, len = gates.size(); i < len; ++i) {
      Var v = s.newVar();
      gates[i]->setVar(v);
   }

   s.assumeRelease();

   for(int i = 0; i < n; ++i){
      for(int j = 0; j < n-1; ++j){
         for(int k = j+1; k < n; ++k){
            Var newV = s.newVar();
            s.addAigCNF(newV, gates[i*n+j]->getVar(), false, gates[i*n+k]->getVar(), false);
            s.assumeProperty(newV, false);
            newV = s.newVar();
            s.addAigCNF(newV, gates[j*n+i]->getVar(), false, gates[k*n+i]->getVar(), false);
            s.assumeProperty(newV, false);
         }
      }
   }

   for(int i = 0; i < n; ++i){
      Var lastV = s.newVar();
      s.addAigCNF(lastV, gates[i*n]->getVar(), true, gates[i*n+1]->getVar(), true);
      for(int j = 2; j < n; ++j){
         Var newV = s.newVar();
         s.addAigCNF(newV, gates[i*n+j]->getVar(), true, lastV, false);
         lastV = newV;
      }
      s.assumeProperty(lastV, false);

      lastV = s.newVar();
      s.addAigCNF(lastV, gates[i]->getVar(), true, gates[n+i]->getVar(), true);
      for(int j = 2; j < n; ++j){
         Var newV = s.newVar();
         s.addAigCNF(newV, gates[j*n+i]->getVar(), true, lastV, false);
         lastV = newV;
      }
      s.assumeProperty(lastV, false);
   }

}

void
assign(SatSolver& sol, int m, int s, int n){
   sol.assumeProperty(gates[m*n+s]->getVar(), true);
}

void
assignNot(SatSolver& sol, int m, int s, int n){
   sol.assumeProperty(gates[m*n+s]->getVar(), false);
}

void 
lessThan(SatSolver& sol, int mi, int mj, int n){
   // mi should not seat at n-1 seat
   sol.assumeProperty(gates[mi*n+(n-1)]->getVar(), false);

   for(int i = 0; i < n-1; ++i){
      Var lastV = sol.newVar();
      sol.addAigCNF(lastV, gates[mi*n+i]->getVar(), false, gates[mj*n+i+1]->getVar(), true);
      for(int j = i+2; j < n; ++j){
         Var newV = sol.newVar();
         sol.addAigCNF(newV, lastV, false, gates[mj*n+j]->getVar(), true);
         lastV = newV;
      }
      sol.assumeProperty(lastV, false);
   }
}

void 
adjacent(SatSolver& sol, int mi, int mj, int n){
   for(int i = 0; i < n; ++i){
      if(i == 0){
         Var newV = sol.newVar();
         sol.addAigCNF(newV, gates[mi*n+0]->getVar(), false, gates[mj*n+1]->getVar(), true);
         sol.assumeProperty(newV, false);
      }
      else if(i == n-1){
         Var newV = sol.newVar();
         sol.addAigCNF(newV, gates[mi*n+n-1]->getVar(), false, gates[mj*n+n-2]->getVar(), true);
         sol.assumeProperty(newV, false);
      }
      else{
         Var newV1 = sol.newVar();
         Var newV2 = sol.newVar();
         sol.addAigCNF(newV1, gates[mi*n+i]->getVar(), false, gates[mj*n+i+1]->getVar(), true);
         sol.addAigCNF(newV2, newV1, false, gates[mj*n+i-1]->getVar(), true);
         sol.assumeProperty(newV2, false);
      }
   }
}

void 
adjacentNot(SatSolver& sol, int mi, int mj, int n){
   for(int i = 0; i < n; ++i){
      if(i == 0){
         Var newV = sol.newVar();
         sol.addAigCNF(newV, gates[mi*n+0]->getVar(), false, gates[mj*n+1]->getVar(), false);
         sol.assumeProperty(newV, false);
      }
      else if(i == n-1){
         Var newV = sol.newVar();
         sol.addAigCNF(newV, gates[mi*n+n-1]->getVar(), false, gates[mj*n+n-2]->getVar(), false);
         sol.assumeProperty(newV, false);
      }
      else{
         Var newV1 = sol.newVar();
         Var newV2 = sol.newVar();
         sol.addAigCNF(newV1, gates[mi*n+i]->getVar(), false, gates[mj*n+i+1]->getVar(), false);
         sol.addAigCNF(newV2, newV1, false, gates[mj*n+i-1]->getVar(), false);
         sol.assumeProperty(newV2, false);
      }
   }
}

void 
reportResult(const SatSolver& sol, bool result, int n){
   if(result){
      cout << "Satisfiable assignment:" << endl;
      for(int i = 0; i < n*n; ++i){
         if(sol.getValue(gates[i]->getVar())){
            if(i/n == n-1){
               cout << i/n << "(" << i%n << ")" << endl;
            }
            else{
               cout << i/n << "(" << i%n << "), ";
            }
         }
      }
   }
   else{
      cout << "No satisfiable assignment can be found." << endl;
   }
}


int main(int argc, char **argv)
{
   if (argc != 2) {
      cerr << "Error: Missing input file!!" << endl;
      exit(-1);
   }

   // ifstream inputFile(argv[1]);
   string file = argv[1];
   fstream f;
   if(file.find('/') == string::npos) file = "./" + file;
   f.open(file, ios::in);
   if(!f){
      cerr << "Error opening the file!" << endl;
      return 0;
   }

   string line;
   int N = -1;
   SatSolver sol;
   bool result;

   while(getline(f, line)){
      if(N < 0){
         N = stoi(line);
         initCircuit(N*N);
         sol.initialize();
         genProofModel(sol, N);
         continue;
      }
      if(line[1] == 's'){
         int m, s;
         m = stoi(line.substr(line.find('(') + 1, line.find(',') - line.find('(') - 1));
         s = stoi(line.substr(line.find(',') + 2, line.find(')') - line.find(',') - 2));
         if(line[6] == 'N'){
            // AssignNot
            // cout << m << s << endl;
            assignNot(sol, m, s, N);
         }
         else{
            // Assign
            // cout << m << s << endl;
            assign(sol, m, s, N);
         }
      }
      else if(line[1] == 'e'){
         int mi, mj;
         mi = stoi(line.substr(line.find('(') + 1, line.find(',') - line.find('(') - 1));
         mj = stoi(line.substr(line.find(',') + 2, line.find(')') - line.find(',') - 2));
         // LessThan
         // cout << mi << mj << endl;
         lessThan(sol, mi, mj, N);
      }
      else if(line[1] == 'd'){
         int mi, mj;
         mi = stoi(line.substr(line.find('(') + 1, line.find(',') - line.find('(') - 1));
         mj = stoi(line.substr(line.find(',') + 2, line.find(')') - line.find(',') - 2));
         if(line[6] == 'N'){
            // AdjacentNot
            // cout << mi << mj << endl;
            adjacentNot(sol, mi, mj, N);
         }
         else{
            // Adjacent
            // cout << mi << mj << endl;
            adjacent(sol, mi, mj, N);
         }
      }
      else{
         cerr << "INVALID CONSTRAINT SYNTAX !!!" << endl;
      }
   }
   f.close();

   result = sol.assumpSolve();
   reportResult(sol, result, N);

   return 0;
}

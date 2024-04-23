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
            cout << m << s << endl;
            assignNot(sol, m, s, N);
         }
         else{
            // Assign
            cout << m << s << endl;
            assign(sol, m, s, N);
         }
      }
      else if(line[1] == 'e'){
         int mi, mj;
         mi = stoi(line.substr(line.find('(') + 1, line.find(',') - line.find('(') - 1));
         mj = stoi(line.substr(line.find(',') + 2, line.find(')') - line.find(',') - 2));
         // LessThan
         cout << mi << mj << endl;
      }
      else if(line[1] == 'd'){
         int mi, mj;
         mi = stoi(line.substr(line.find('(') + 1, line.find(',') - line.find('(') - 1));
         mj = stoi(line.substr(line.find(',') + 2, line.find(')') - line.find(',') - 2));
         if(line[6] == 'N'){
            // AdjacentNot
            cout << mi << mj << endl;
         }
         else{
            // Adjacent
            cout << mi << mj << endl;
         }
      }
      else{
         cerr << "INVALID CONSTRAINT SYNTAX !!!" << endl;
      }

      // cout << line << endl;
   }
   f.close();
   return 0;
}

/****************************************************************************
  FileName     [ proveBdd.cpp ]
  PackageName  [ prove ]
  Synopsis     [ For BDD-based verification ]
  Author       [ ]
  Copyright    [ Copyright(c) 2023-present DVLab, GIEE, NTU, Taiwan ]
****************************************************************************/

#include "bddMgrV.h"
#include "gvMsg.h"
// #include "gvNtk.h"
#include <iomanip>
#include <iostream>
#include <vector>

#include "cirGate.h"
#include "cirMgr.h"
#include "util.h"

void
BddMgrV::buildPInitialState() {
    // TODO : remember to set _initState
    // Hint : use "cirMgr" to get the network info from the manager 
    // set initial state to all zero

    BddNodeV init = BddNodeV::_one;
    for(unsigned i = 0, n = cirMgr->getNumLATCHs(); i < n; ++i){
        init &= ~getSupport(cirMgr->getRo(i)->getGid());
    }
    _initState = init;
}

void
BddMgrV::buildPTransRelation() {
    // TODO : remember to set _tr, _tri
    // Hint : use "cirMgr" to get the network info from the manager

    // TR(Y,X,I) = ~(Y^delta(X,I))
    _tri = BddNodeV::_one;
    for(unsigned i = 0, n = cirMgr->getNumLATCHs(); i < n; ++i){
        CirGateV d = cirMgr->getRi(i)->getIn0();
        BddNodeV delta = getBddNodeV(d.gateId());
        if(d.isInv()) delta = ~delta;
        _tri &= ~(getBddNodeV(cirMgr->getRi(i)->getName()) ^ delta);
    }

    _tr = _tri;
    for(unsigned i = 0, n = cirMgr->getNumPIs(); i < n; ++i){
        _tr = _tr.exist(getBddNodeV(cirMgr->getPi(i)->getGid()).getLevel());
    }
}

void
BddMgrV::buildPImage(int level) {
    // TODO : remember to add _reachStates and set _isFixed
    // Hint : use "cirMgr" to get the network info from the manager
    // note:: _reachStates record the set of reachable states

    for(int i = 0; i < level; ++i){
        if(!_isFixed){
            // S(Y,X) = TR(Y,X) & R(X)
            BddNodeV S = getPTr() & getPReachState();

            // S(Y) = exist.x (TR(Y,X) & S(X))
            for(unsigned i = 0, n = cirMgr->getNumLATCHs(); i < n; ++i){
                // S = S.exist(cirMgr->getRi(i)->getGid());
                S = S.exist(getBddNodeV(cirMgr->getRo(i)->getGid()).getLevel());
            }

            // S(X) = S(Y)
            unsigned t = getBddNodeV(cirMgr->getRo(0)->getGid()).getLevel();
            unsigned f = t + cirMgr->getNumLATCHs();
            // unsigned t = cirMgr->getRi(0)->getGid();
            bool isMoved;
            S = S.nodeMove(f, t, isMoved);

            // set _isFixed
            BddNodeV R_r = getPReachState();
            if((R_r|S) == R_r) _isFixed = true;
            else _reachStates.push_back(R_r|S);
        }
    }
    if(_isFixed){
        cout << "Fixed point is reached (time : " << _reachStates.size() << ")" << endl; 
    }
    
}

void
BddMgrV::runPCheckProperty(const string& name, BddNodeV monitor) {
    // TODO : prove the correctness of AG(~monitor)
    // Hint : use "cirMgr" to get the network info from the manager

    BddNodeV check = monitor & getPReachState();

    if (check.countCube() != 0) // case1: have counter example
    {
        cout << "Monitor \"" << name << "\" is violated." << endl;
        cout << "Counter Example:" << endl;
        int i = _reachStates.size() - 1;
        while ((monitor & _reachStates[i]).countCube() != 0)
        {
            i--;
            if (i == 0)
                break;
        }
        i++;
        BddNodeV S = check.getCube(), V, Si, Sy;
        bool isMoved;
        vector<string> output;

        // propagate backward to get counter example
        for (i; i >= 0; i--)
        {
            // Step1: Vn(I) = exist Y,X "TRI & Sn+1(X->Y)"
            Si = S;
            Sy = S;
            // seperate Sn+1(X,I) in to Sy(X->Y) and Si(I)
            for (unsigned i = 0, n = cirMgr->getNumPIs(); i < n; i++)
            {
                Sy = Sy.exist(getBddNodeV((cirMgr->getPi(i))->getGid()).getLevel()); // exist I to get Sy(X)
            }
            Sy = Sy.nodeMove(getBddNodeV((cirMgr->getRo(0))->getGid()).getLevel(), getBddNodeV((cirMgr->getRi(0))->getName()).getLevel(), isMoved);
            for (unsigned i = 0, n = cirMgr->getNumLATCHs(); i < n; i++)
            {
                Si = Si.exist(getBddNodeV((cirMgr->getRo(i))->getGid()).getLevel()); // exist X
            }
            S = Sy & Si;
            V = getPTri() & S;
            for (unsigned i = 0, n = cirMgr->getNumLATCHs(); i < n; i++)
            {
                V = V.exist(getBddNodeV((cirMgr->getRo(i))->getGid()).getLevel());  // exist X
                V = V.exist(getBddNodeV((cirMgr->getRi(i))->getName()).getLevel()); // exist Y
            }

            // Step2: Sn(X) = exist Y,I "TRI & Sn+1(X->Y) & Vn"
            S = (getPTri() & S & V);
            for (unsigned i = 0, n = cirMgr->getNumPIs(); i < n; i++)
            {
                S = S.exist(getBddNodeV((cirMgr->getPi(i))->getGid()).getLevel()); // exist I
            }
            for (unsigned i = 0, n = cirMgr->getNumLATCHs(); i < n; i++)
            {
                S = S.exist(getBddNodeV((cirMgr->getRi(i))->getName()).getLevel()); // exist Y
            }

            // Step3: cout
            string message;
            if (V == BddNodeV::_zero) // no contradiction occur
            {
                message = "No counter example. Contradiction occur in initial state.";
                output.push_back(message);
                break;
            }
            else if (V == BddNodeV::_one) // input can be random input
            {
                message = to_string(i) + ": " + "input can be any random input";
                output.push_back(message);
            }
            else
            {
                message = to_string(i) + ": " + V.getCube().toString();
                output.push_back(message);
            }
        }
        for (int j = output.size() - 1; j >= 0; j--)
            cout << output[j] << endl;
    }
    else if (_isFixed) // case2: not find but not yet fixed
    {
        cout << "Monitor \"" << name << "\" is safe." << endl;
    }
    else // case3: safe
    {
        cout << "Monitor \"" << name << "\" is safe up to time " << _reachStates.size() - 1 << "." << endl;
    }
}

BddNodeV
BddMgrV::find_ns(BddNodeV cs) {}

BddNodeV
BddMgrV::ns_to_cs(BddNodeV ns) {}

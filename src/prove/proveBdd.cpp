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
        init &= ~getSupport(cirMgr->getRi(i)->getGid());
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
        CirRiGate* d = cirMgr->getRi(i);
        BddNodeV delta = getBddNodeV(d->getGid());
        if(d->getIn0().isInv()) delta = ~delta;
        _tri &= ~(getSupport(d->getGid() + cirMgr->getNumLATCHs()) ^ delta);
    }

    _tr = _tri;
    for(unsigned i = 0, n = cirMgr->getNumPIs(); i < n; ++i){
        _tr = _tr.exist(cirMgr->getPi(i)->getGid());
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
                S = S.exist(cirMgr->getRi(i)->getGid());
            }

            // S(X) = S(Y)
            unsigned f = cirMgr->getRi(0)->getGid() + cirMgr->getNumLATCHs();
            unsigned t = cirMgr->getRi(0)->getGid();
            bool isMoved;
            S = S.nodeMove(f, t, isMoved);

            // set _isFixed
            BddNodeV R_r = getPReachState();
            if((R_r|S) == R_r) _isFixed = true;
            else _reachStates.push_back(R_r|S);
        }
    }
    if(_isFixed){
        cout << "Fixed point is reached (time : " << _reachStates.size()-1 << ")" << endl; 
    }
    
}

void
BddMgrV::runPCheckProperty(const string& name, BddNodeV monitor) {
    // TODO : prove the correctness of AG(~monitor)
    // Hint : use "cirMgr" to get the network info from the manager
}

BddNodeV
BddMgrV::find_ns(BddNodeV cs) {}

BddNodeV
BddMgrV::ns_to_cs(BddNodeV ns) {}

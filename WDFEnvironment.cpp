/*
  ==============================================================================

    WDFEnvironment.cpp
    Created: 29 Apr 2021 4:54:06pm
    Author:  Stef

  ==============================================================================
*/

#include "WDFEnvironment.h"


WdfEnvironment::WdfEnvironment(){
    setSamplerate(44100*2);
    subtreeCount = 1;
    subtreeEntryNodes = new wdfTreeNode*[subtreeCount];
}
void WdfEnvironment::setInputValue( double signalIn ){
    
}
double WdfEnvironment::getOutputValue( ){
    
}
int WdfEnvironment::setRootMatrData( matData* rootMatrixData, double *Rp ){
    if(getRNodeMatLambda != nullptr){
        Smat = getRNodeMatLambda(rootMatrixData,Rp);
    }
    for ( unsigned int ii = 0; ii < Smat.n_cols; ++ii ) {
            for ( unsigned int jj = 0; jj < Smat.n_rows; ++jj ) {
                rootMatrixData->Smat.at(ii, jj) = Smat.at(ii,jj);
            }
    }
}
const char* WdfEnvironment::getTreeIdentifier( ){
    
}
void WdfEnvironment::setParam( size_t paramID, double paramValue ){
    
}

void WdfEnvironment::setSubtreeEntryNodes(wdfTreeNode * node){
    subtreeEntryNodes[0] = node;
}

void WdfEnvironment::setSubtreeEntryNodes(std::vector<wdfTreeNode*> nodes){
    subtreeCount = nodes.size();
    subtreeEntryNodes = new wdfTreeNode*[subtreeCount];
    for(auto i=0; i<nodes.size(); i++){
        subtreeEntryNodes[i] = nodes[i];
    }
    root.reset(new wdfRootRtype(subtreeCount));
    Rp = new double[subtreeCount]();
}

void WdfEnvironment::setSubtreeEntryNodes(std::vector<wdfTreeNode*> nodes, std::vector<int> nlList){
    subtreeCount = nodes.size();
    subtreeEntryNodes = new wdfTreeNode*[subtreeCount];
    for(auto i=0; i<nodes.size(); i++){
        subtreeEntryNodes[i] = nodes[i];
    }
    
    root.reset(new wdfRootNL(subtreeCount,nlList,1));
    Rp = new double[subtreeCount]();
}

void WdfEnvironment::setRoot(wdfRootNode * r){
    root.reset(new wdfRootSimple(r));
    Rp = new double[subtreeCount]();
}

void WdfEnvironment::addParam(paramData p){
    params.push_back(p);
}

void WdfEnvironment::setSMat(mat newSMat){
    Smat = newSMat;
}

void WdfEnvironment::setSampleRateWDF(double newSampleRate){
    setSamplerate(newSampleRate);
}

/*
  ==============================================================================

    WDFEnvironment.h
    Created: 29 Apr 2021 4:54:06pm
    Author:  Stef

  ==============================================================================
*/

#pragma once
#include "../rt-wdf_lib/Libs/rt-wdf/rt-wdf.h"


class WdfEnvironment : public wdfTree
{
public:
    WdfEnvironment();
    void setInputValue( double signalIn );
    double getOutputValue( );
    int setRootMatrData( matData* rootMatrixData, double *Rp );
    const char* getTreeIdentifier( );
    void setParam( size_t paramID, double paramValue );
    
    void setSubtreeEntryNodes(wdfTreeNode * node);
    
    void setSubtreeEntryNodes(std::vector<wdfTreeNode*> nodes);
    
    void setRoot(wdfRootNode * r);
    
    void addParam(paramData p);
    
    void setSMat(mat newSMat);
    
    std::function<mat(double*)> getRNodeMatLambda;
private:
    mat Smat;
};

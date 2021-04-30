/*
  ==============================================================================

    VoltageSource.h
    Created: 29 Apr 2021 4:45:58pm
    Author:  Stef

  ==============================================================================
*/

#pragma once
#include "OnePortComponent.h"

class VoltageSource : public OnePortComponent
{
public:
    VoltageSource();
    wdfTreeNode* createWDFComponent() override;
    ComponentType getComponentType() override;
    void setVs(double newVs);
    double getVs();
    void setRs(double newRs);
    double getRs();
    
private:
    double Vs;
    double Rs;
};
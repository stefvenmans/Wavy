/*
  ==============================================================================

    Capacitor.h
    Created: 29 Apr 2021 4:45:38pm
    Author:  Stef

  ==============================================================================
*/

#pragma once
#include "OnePortComponent.h"

class Capacitor : public OnePortComponent
{
public:
    Capacitor();
    wdfTreeNode* createWDFComponent() override;
    ComponentType getComponentType() override;
    void setC(double newC);
    double getC();
    void setT(double newT);
    double getT();
    
private:
    double C;
    double T;
};

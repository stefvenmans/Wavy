/*
  ==============================================================================

    Inductor.h
    Created: 29 Apr 2021 4:45:38pm
    Author:  Stef

  ==============================================================================
*/

#pragma once
#include "OnePortComponent.h"

class Inductor : public OnePortComponent
{
public:
    Inductor();
    wdfTreeNode* createWDFComponent() override;
    ComponentType getComponentType() override;
    void setL(double newL);
    double getL();
    void setT(double newT);
    double getT();
    juce::String getInfo() override;
    
private:
    double L;
    double T;
};


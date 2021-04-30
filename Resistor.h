/*
  ==============================================================================

    Resistor.h
    Created: 29 Apr 2021 4:45:28pm
    Author:  Stef

  ==============================================================================
*/

#pragma once
#include "OnePortComponent.h"

class Resistor : public OnePortComponent
{
public:
    Resistor();
    wdfTreeNode* createWDFComponent() override;
    ComponentType getComponentType() override;
    void setR(double newR);
    double getR();
private:
    double R;
};

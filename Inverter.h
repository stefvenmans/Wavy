/*
  ==============================================================================

    Inverter.h
    Created: 29 Apr 2021 4:46:52pm
    Author:  Stef

  ==============================================================================
*/

#pragma once
#include "TwoPortComponent.h"

class Inverter : public TwoPortComponent
{
public:
    Inverter();
    wdfTreeNode* createWDFComponent() override;
    ComponentType getComponentType() override;
};

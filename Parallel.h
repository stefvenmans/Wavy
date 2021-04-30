/*
  ==============================================================================

    Parallel.h
    Created: 29 Apr 2021 5:18:24pm
    Author:  Stef

  ==============================================================================
*/

#pragma once
#include "ThreePortComponent.h"

class Parallel : public ThreePortComponent
{
public:
    Parallel();
    wdfTreeNode * createWDFComponent() override;
    ComponentType getComponentType() override;
};

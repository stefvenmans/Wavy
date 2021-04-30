/*
  ==============================================================================

    Series.h
    Created: 29 Apr 2021 4:48:09pm
    Author:  Stef

  ==============================================================================
*/

#pragma once
#include "ThreePortComponent.h"

class Series : public ThreePortComponent
{
public:
    Series();
    wdfTreeNode * createWDFComponent() override;
    ComponentType getComponentType() override;
};

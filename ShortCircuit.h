/*
  ==============================================================================

    ShortCircuit.h
    Created: 29 Apr 2021 4:49:07pm
    Author:  Stef

  ==============================================================================
*/

#pragma once
#include "SimpleRootComponent.h"

class ShortCircuit : public SimpleRootComponent
{
public:
    ShortCircuit();
    wdfRootNode * createWDFComponent() override;
    ComponentType getComponentType() override;
};

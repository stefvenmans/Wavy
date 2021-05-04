/*
  ==============================================================================

    NonLinearComponent.h
    Created: 29 Apr 2021 4:50:48pm
    Author:  Stef

  ==============================================================================
*/

#pragma once
#include "Config.h"
#include "CircuitComponent.h"

class NonLinearComponent : public CircuitComponent
{
public:
    NonLinearComponent(juce::String svgFileName);
    int isRootOrNonLin() override; 
    
    
};

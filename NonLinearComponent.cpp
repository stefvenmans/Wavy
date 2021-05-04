/*
  ==============================================================================

    NonLinearComponent.cpp
    Created: 29 Apr 2021 4:50:48pm
    Author:  Stef

  ==============================================================================
*/

#include "NonLinearComponent.h"

NonLinearComponent::NonLinearComponent(juce::String svgFileName) : CircuitComponent(svgFileName)
{
    
}

int NonLinearComponent::isRootOrNonLin(){
    return 2;
}

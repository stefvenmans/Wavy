/*
  ==============================================================================

    OnePortComponent.h
    Created: 29 Apr 2021 4:46:26pm
    Author:  Stef

  ==============================================================================
*/

#pragma once
#include "Config.h"
#include "AdaptedLeafComponent.h"

class OnePortComponent : public AdaptedLeafComponent
{
public:
    OnePortComponent(juce::String svgFileName);
    void paint(juce::Graphics& g) override;
    void connect(CircuitComponent* c) override ;
    
protected:
    juce::Line<float> wBLine;
};

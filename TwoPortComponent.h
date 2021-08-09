/*
  ==============================================================================

    TwoPortComponent.h
    Created: 29 Apr 2021 4:46:38pm
    Author:  Stef

  ==============================================================================
*/

#pragma once
#include "AdaptedLeafComponent.h"

class TwoPortComponent : public AdaptedLeafComponent
{
public:
    TwoPortComponent(juce::String svgFileName);
    void paint(juce::Graphics& g) override;
    void connectChild(AdaptedLeafComponent* newChild);
    void disconnectChild();
    int connect(CircuitComponent* c) override ;
protected:
    AdaptedLeafComponent* child;
    std::vector<juce::Line<float>> portLines;
    
};

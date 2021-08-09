/*
  ==============================================================================

    ThreePortComponent.h
    Created: 29 Apr 2021 4:48:00pm
    Author:  Stef

  ==============================================================================
*/

#pragma once
#include "AdaptedLeafComponent.h"

class ThreePortComponent : public AdaptedLeafComponent
{
public:
    ThreePortComponent(juce::String svgFileName);
    void paint(juce::Graphics& g) override;
    void connectLeftChild(AdaptedLeafComponent* newLeftChild);
    void connectRightChild(AdaptedLeafComponent* newRightChild);
    void disconnectLeftChild();
    void disconnectRightChild();
    int connect(CircuitComponent* c) override;
    juce::String getInfo() override;
    
protected:
    AdaptedLeafComponent* leftChild;
    AdaptedLeafComponent* rightChild;
    std::vector<juce::Line<float>> portLines;
};

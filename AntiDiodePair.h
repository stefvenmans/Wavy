/*
  ==============================================================================

    AntiDiodePair.h
    Created: 2 May 2021 6:51:06pm
    Author:  Stef

  ==============================================================================
*/

#pragma once
#include "NonLinearComponent.h"

class AntiDiodePair : public NonLinearComponent
{
public:
    AntiDiodePair();
    void paint(juce::Graphics& g) override;
    int connect(CircuitComponent* c) override ;
    ComponentType getComponentType() override;
};

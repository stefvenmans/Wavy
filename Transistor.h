/*
  ==============================================================================

    Transistor.h
    Created: 2 May 2021 8:52:18pm
    Author:  Stef

  ==============================================================================
*/

#pragma once
#include "NonLinearComponent.h"

class Transistor : public NonLinearComponent
{
public:
    Transistor();
    void paint (juce::Graphics& g) override;
    int getCollums() override;
};

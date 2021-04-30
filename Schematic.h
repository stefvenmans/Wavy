/*
  ==============================================================================

    Schematic.h
    Created: 29 Apr 2021 4:53:19pm
    Author:  Stef

  ==============================================================================
*/

#pragma once
#include "Config.h"

class Schematic : public juce::Component
{
public:
    Schematic();
    void paint (juce::Graphics& g) override;
    void mouseDown(const juce::MouseEvent& e) override;
    
private:
    juce::Label l1;
};

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
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;
    std::function<void(void)> hidePropertyPanelCallbck;
    
private:
    juce::Label l1;
    //Selection area square
    juce::Rectangle<int> selectRect;
    bool isSelecting = false;
    
};

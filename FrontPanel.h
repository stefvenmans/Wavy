/*
  ==============================================================================

    FrontPanel.h
    Created: 29 Apr 2021 4:52:46pm
    Author:  Stef

  ==============================================================================
*/

#pragma once
#include "Config.h"

class FrontPanel : public juce::Component
{
public:
    FrontPanel();
    void paint (juce::Graphics& g) override;
    void addNewComponent(juce::Component* c);
    void resized() override;
private:
    juce::OwnedArray<juce::Component> components;
};

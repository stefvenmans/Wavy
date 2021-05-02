/*
  ==============================================================================

    ControlPanel.h
    Created: 2 May 2021 10:53:47am
    Author:  Stef

  ==============================================================================
*/

#pragma once
#include "Config.h"

class ControlPanel : public juce::Component
{
public:
    ControlPanel();
    void paint (juce::Graphics& g) override;
private:
    std::unique_ptr<juce::Drawable> svgDrawable;
};

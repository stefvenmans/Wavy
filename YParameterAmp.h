/*
  ==============================================================================

    YParameterAmp.h
    Created: 29 Apr 2021 4:47:16pm
    Author:  Stef

  ==============================================================================
*/

#pragma once
#include "TwoPortComponent.h"

class YParameterAmp : public TwoPortComponent
{
public:
    YParameterAmp();
    wdfTreeNode* createWDFComponent() override;
    ComponentType getComponentType() override;
    void paint(juce::Graphics& g)  override;
};

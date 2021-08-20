/*
  ==============================================================================

    AdaptedLeafComponent.h
    Created: 29 Apr 2021 4:45:06pm
    Author:  Stef

  ==============================================================================
*/

#pragma once
#include "Config.h"
#include "CircuitComponent.h"

class AdaptedLeafComponent : public CircuitComponent
{
public:
    AdaptedLeafComponent(juce::String svgFileName);
    void paint(juce::Graphics& g) override;
    virtual wdfTreeNode* createWDFComponent();
    virtual wdfTreeNode* getWDFComponent();
    int isRootOrNonLin() override;
    
protected:
    std::unique_ptr<wdfTreeNode> treeNode;
    std::vector<juce::Line<float>> portArrowsA;
    std::vector<juce::Line<float>> portArrowsB;
    std::vector<bool> isAdapted;
};

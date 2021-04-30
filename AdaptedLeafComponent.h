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
    virtual wdfTreeNode* createWDFComponent();
    virtual wdfTreeNode* getWDFComponent();
    
protected:
    std::unique_ptr<wdfTreeNode> treeNode;
    bool isAdapted = true;
};

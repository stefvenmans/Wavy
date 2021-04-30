/*
  ==============================================================================

    AdaptedLeafComponent.cpp
    Created: 29 Apr 2021 4:45:06pm
    Author:  Stef

  ==============================================================================
*/

#include "AdaptedLeafComponent.h"

    AdaptedLeafComponent::AdaptedLeafComponent(juce::String svgFileName) : CircuitComponent(svgFileName)
    {
        
        
    }
    
    wdfTreeNode* AdaptedLeafComponent::createWDFComponent(){
        return treeNode.get();
    }

    wdfTreeNode* AdaptedLeafComponent::getWDFComponent(){
        return treeNode.get();
    }
  

/*
  ==============================================================================

    SimpleRootComponent.h
    Created: 29 Apr 2021 4:48:37pm
    Author:  Stef

  ==============================================================================
*/

#pragma once
#include "CircuitComponent.h"
#include "AdaptedLeafComponent.h"

class SimpleRootComponent : public CircuitComponent
{
public:
    SimpleRootComponent(juce::String svgFileName);
    void paint(juce::Graphics& g) override;
    virtual wdfRootNode * createWDFComponent() ;
    void connect(CircuitComponent* c) override ;
    AdaptedLeafComponent* getChildComponent();
    
protected:
    AdaptedLeafComponent* child;
    std::unique_ptr<wdfRootNode> root;
    
private:
    juce::Line<float> wBLine;
};

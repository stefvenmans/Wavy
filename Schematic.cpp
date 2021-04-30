/*
  ==============================================================================

    Schematic.cpp
    Created: 29 Apr 2021 4:53:19pm
    Author:  Stef

  ==============================================================================
*/

#include "Schematic.h"

Schematic::Schematic(){
    l1.setText("Dummy text",juce::NotificationType::dontSendNotification);
    addAndMakeVisible(l1);
    l1.setBounds((820*5)/2,(580*5)/2,500,80);
    
}

void Schematic::paint (juce::Graphics& g){
    g.setColour(juce::Colours::grey);
    g.fillAll(juce::Colours::lightgrey);
    g.drawRect(juce::Rectangle<float>(5000/2-300,5000/2,50,50),15);
    
    
    float x = 1000;
    float y = 1000;
    juce::Path sineGraph;
    
    sineGraph.startNewSubPath(x, y);

    auto numberOfPoints = 1000;
    for (int i = 1; i < numberOfPoints; ++i)
    {
 // calculate or optain the coordinates x and y of the current point
        
        x += 0.1;
        y = 1000.0+100.0*std::sin(x*(juce::MathConstants<double>::pi*2.0)/100.0);
        sineGraph.lineTo(x, y);
    }

    float lineThickness = 0.75f;
    g.strokePath (sineGraph, juce::PathStrokeType(lineThickness));
    
    
}

void Schematic::mouseDown(const juce::MouseEvent& e){
       std::cout << "mouse down was called" << std::endl;
   }
    

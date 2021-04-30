/*
  ==============================================================================

    FrontPanel.cpp
    Created: 29 Apr 2021 4:52:46pm
    Author:  Stef

  ==============================================================================
*/

#include "FrontPanel.h"

FrontPanel::FrontPanel(){
    
}
void FrontPanel::paint (juce::Graphics& g){
    g.setColour(juce::Colours::grey);
    g.fillAll(juce::Colours::grey);
}
void FrontPanel::addNewComponent(juce::Component* c){
    int x = 0;
    ((juce::Slider*)(c))->setSliderStyle(juce::Slider::SliderStyle::Rotary);
    if(components.isEmpty()) x = 15;
    else x += components.getLast()->getX() + 115;
    ((juce::Slider*)(c))->setBounds(x,15,100,100);
    ((juce::Slider*)(c))->setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true,50,20);
    
    addAndMakeVisible(components.add(c));
}
void FrontPanel::resized(){
    
}
    

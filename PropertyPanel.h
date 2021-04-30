/*
  ==============================================================================

    PropertyPanel.h
    Created: 29 Apr 2021 4:53:39pm
    Author:  Stef

  ==============================================================================
*/

#pragma once
#include "Config.h"
#include "AdaptedLeafComponent.h"
#include "OnePortComponent.h"
#include "Resistor.h"
#include "Capacitor.h"
#include "VoltageSource.h"
#include "TwoPortComponent.h"
#include "Inverter.h"
#include "YParameterAmp.h"
#include "ThreePortComponent.h"
#include "Series.h"
#include "Parallel.h"
#include "SimpleRootComponent.h"
#include "IdealVoltageSource.h"
#include "ShortCircuit.h"
#include "RNodeRootComponent.h"

class PropertyPanel :   public juce::Component,
                        public juce::Label::Listener,
                        public juce::Button::Listener
{
public:
    PropertyPanel();
    void paint (juce::Graphics& g) override;
    void setPropertiesForComponent(CircuitComponent* c);
    void labelTextChanged (juce::Label *labelThatHasChanged) override;
    void setOutputCallbackFunction(std::function<void(CircuitComponent* c)> callback);
    void setInputCallbackFunction(std::function<void(CircuitComponent*cc)> callback);
    void buttonClicked (juce::Button * b) override;
    
private:
    juce::Label componentName;
    juce::Label prop1;
    juce::Label prop2;
    CircuitComponent* componentLastSelected = nullptr;
    juce::ToggleButton setOutput;
    juce::ToggleButton setInput;
    juce::TextButton addNullorButton;
    std::function<void(CircuitComponent* c)> setOutputOfCircuit;
    std::function<void(CircuitComponent* c)> setInputOfCircuit;
};

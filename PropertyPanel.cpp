/*
  ==============================================================================

    PropertyPanel.cpp
    Created: 29 Apr 2021 4:53:39pm
    Author:  Stef

  ==============================================================================
*/

#include "PropertyPanel.h"

PropertyPanel::PropertyPanel()
{
    addAndMakeVisible(componentName);
    componentName.setBounds(5,0,100,20);
    
    addChildComponent(prop1);
    prop1.setEditable(true);
    prop1.setBounds(5,30,100,20);
    prop1.addListener(this);
    
    addChildComponent(prop2);
    prop2.setEditable(true);
    prop2.setBounds(5,60,100,20);
    prop2.addListener(this);
    
    addAndMakeVisible(setOutput);
    setOutput.setBounds(5,100,25,25);
    setOutput.addListener(this);
    
    addAndMakeVisible(setInput);
    setInput.setBounds(50,100,25,25);
    setInput.addListener(this);
    
    addChildComponent(addNullorButton);
    addNullorButton.setBounds(100,20,50,50);
    addNullorButton.addListener(this);
}

void PropertyPanel::paint (juce::Graphics& g) {
    g.setColour(juce::Colours::darkgrey);
    g.fillAll();
}

void PropertyPanel::setPropertiesForComponent(CircuitComponent* c){
    std::cout << "propertyPanel called for : " << c->getComponentType() << std::endl;
    componentLastSelected = c;
    switch(c->getComponentType()){
        case L_RES:
            std::cout << " i am a resistor" << std::endl;
            componentName.setText("Resistor",juce::NotificationType::dontSendNotification);
            prop1.setText(juce::String(((Resistor*)c)->getR()),juce::NotificationType::dontSendNotification);
            prop1.setVisible(true);
            break;
        case L_CAP:
            std::cout << " i am a capacitor" << std::endl;
            componentName.setText("Capacitor",juce::NotificationType::dontSendNotification);
            prop1.setText(juce::String(((Capacitor*)c)->getC()),juce::NotificationType::dontSendNotification);
            prop1.setVisible(true);
            break;
        case L_VOL:
            componentName.setText("Resistive Voltage Source",juce::NotificationType::dontSendNotification);
            prop1.setText(juce::String(((VoltageSource*)c)->getVs()),juce::NotificationType::dontSendNotification);
            prop1.setVisible(true);
            prop2.setText(juce::String(((VoltageSource*)c)->getRs()),juce::NotificationType::dontSendNotification);
            prop2.setVisible(true);
            break;
        case A_INV:
            componentName.setText("Inverter",juce::NotificationType::dontSendNotification);
            prop1.setVisible(false);
            break;
        case A_SER:
            componentName.setText("Serries",juce::NotificationType::dontSendNotification);
            prop1.setVisible(false);
            break;
        case A_PAR:
            componentName.setText("Parallel",juce::NotificationType::dontSendNotification);
            prop1.setVisible(false);
            break;
            
        case SR_VOL:
            std::cout << " i am a ROOT : voltage source" << std::endl;
            componentName.setText("ROOT Voltage Source",juce::NotificationType::dontSendNotification);
            prop1.setText(juce::String(((IdealVoltageSource*)c)->getVs()),juce::NotificationType::dontSendNotification);
            prop1.setVisible(true);
            break;
        case R_RNODE:
            prop1.setVisible(false);
            prop2.setVisible(false);
            componentName.setText("R Node",juce::NotificationType::dontSendNotification);
            addNullorButton.setVisible(true);
            break;
    }
    
}

void PropertyPanel::labelTextChanged (juce::Label *labelThatHasChanged) {
    switch(componentLastSelected->getComponentType()){
        case L_RES:
            ((Resistor*)componentLastSelected)->setR(prop1.getText().getDoubleValue());
            break;
        case L_CAP:
            ((Capacitor*)componentLastSelected)->setC(prop1.getText().getDoubleValue());
            break;
        case L_VOL:
            if (labelThatHasChanged == &prop1){
                ((VoltageSource*)componentLastSelected)->setVs(prop1.getText().getDoubleValue());
            }
            else if (labelThatHasChanged == &prop2){
                ((VoltageSource*)componentLastSelected)->setRs(prop2.getText().getDoubleValue());
            }
            break;
        case A_INV:
            
            break;
        case A_SER:
            
            break;
        case A_PAR:
            
            break;
        case SR_VOL:
            ((IdealVoltageSource*)componentLastSelected)->setVs(prop1.getText().getDoubleValue());
            break;
            
    }
}

void PropertyPanel::setOutputCallbackFunction(std::function<void(CircuitComponent* c)> callback){
    setOutputOfCircuit = callback;
}

void PropertyPanel::setInputCallbackFunction(std::function<void(CircuitComponent*cc)> callback){
    setInputOfCircuit = callback;
}

void PropertyPanel::buttonClicked (juce::Button * b) {
    //std::cout << "togle state changed" << std::endl;
    if(b == &setOutput){
        if(setOutput.getToggleState()==true){
            std::cout << "output will be set" << std::endl;
            setOutputOfCircuit(componentLastSelected);
        }
    }
    else if(b == &setInput){
        if(setInput.getToggleState()==true){
            std::cout << "input will be set" << std::endl;
            setInputOfCircuit(componentLastSelected);
        }
    }
    else if(b == &addNullorButton){
        ((RNodeRootComponent*)componentLastSelected)->createNullorStamps();
    }
}
    

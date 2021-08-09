/*
  ==============================================================================

    IdealVoltageSource.cpp
    Created: 29 Apr 2021 4:48:50pm
    Author:  Stef

  ==============================================================================
*/

#include "IdealVoltageSource.h"

IdealVoltageSource::IdealVoltageSource() : SimpleRootComponent("idealvol.svg")
{
    Vs = 1;
}

wdfRootNode * IdealVoltageSource::createWDFComponent() {
    if(child != nullptr){
        root.reset(new wdfIdealVSource(Vs) );
    }
    else return nullptr;
    return root.get();
}

ComponentType IdealVoltageSource::getComponentType() {
    return ComponentType::SR_VOL;
}

void IdealVoltageSource::setVs(double newVs){
    Vs = newVs;
}

double IdealVoltageSource::getVs(){
    return Vs;
}

juce::String IdealVoltageSource::getInfo(){
    juce::String result;
    result = juce::String("<") + ComponentTypeString[getComponentType()] +">" + "name=" + getName() + "," + "x="  +  juce::String(getX()) + "," + "y=" + juce::String(getY()) + "," + "ang=" + juce::String(angle) + ",Vs=" + juce::String(getVs());
//    if(child != nullptr) result += "\np1=" + child->getInfo();
//    else result += "\np1=NC";
    result += "</"+ComponentTypeString[getComponentType()]+">";
    return result;
}

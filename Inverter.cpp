/*
  ==============================================================================

    Inverter.cpp
    Created: 29 Apr 2021 4:46:52pm
    Author:  Stef

  ==============================================================================
*/

#include "Inverter.h"

Inverter::Inverter() : TwoPortComponent("inv.svg")
{
    
}

wdfTreeNode* Inverter::createWDFComponent() {
    treeNode.reset(new wdfInverter(child->createWDFComponent()));
    return treeNode.get();
}

ComponentType Inverter::getComponentType() {
    return ComponentType::A_INV;
}

juce::String Inverter::getInfo(){
    juce::String result;
    result = juce::String("<") + ComponentTypeString[getComponentType()] +">" + "name=" + getName() + "," + "x="  +  juce::String(getX()) + "," + "y=" + juce::String(getY()) + "," + "ang=" + juce::String(angle);
    result += "</"+ComponentTypeString[getComponentType()]+">";
    return result;
}


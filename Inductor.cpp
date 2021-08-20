/*
  ==============================================================================

    Inductor.cpp
    Created: 29 Apr 2021 4:45:38pm
    Author:  Stef

  ==============================================================================
*/

#include "Inductor.h"

Inductor::Inductor() : OnePortComponent("ind.svg")
{
    L = 1;
    T = 1;
}

wdfTreeNode* Inductor::createWDFComponent() {
    treeNode.reset(new wdfTerminatedInd(L,T));
    return treeNode.get();
}

ComponentType Inductor::getComponentType() {
    return ComponentType::L_IND;
}

void Inductor::setL(double newL){
    L = newL;
}

double Inductor::getL(){
    return L;
}

void Inductor::setT(double newT){
    T = newT;
}

double Inductor::getT(){
    return T;
}

juce::String Inductor::getInfo(){
    juce::String result;
    result = juce::String("<") + ComponentTypeString[getComponentType()] +">" + "name=" + getName() + "," + "x="  +  juce::String(getX()) + "," + "y=" + juce::String(getY()) + "," + "ang=" + juce::String(angle) + "," + "ind=" + juce::String(getL());
    result += "</"+ComponentTypeString[getComponentType()]+">";
    return result;
}

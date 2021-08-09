/*
  ==============================================================================

    Capacitor.cpp
    Created: 29 Apr 2021 4:45:38pm
    Author:  Stef

  ==============================================================================
*/

#include "Capacitor.h"

Capacitor::Capacitor() : OnePortComponent("cap.svg")
{
    C = 1;
    T = 1;
}

wdfTreeNode* Capacitor::createWDFComponent() {
    treeNode.reset(new wdfTerminatedCap(C,T));
    return treeNode.get();
}

ComponentType Capacitor::getComponentType() {
    return ComponentType::L_CAP;
}

void Capacitor::setC(double newC){
    C = newC;
}

double Capacitor::getC(){
    return C;
}

void Capacitor::setT(double newT){
    T = newT;
}

double Capacitor::getT(){
    return T;
}

juce::String Capacitor::getInfo(){
    juce::String result;
    result = juce::String("<L_CAP>") + "name=" + getName() + "," + "x="  +  juce::String(getX()) + "," + "y=" + juce::String(getY()) + "," + "ang=" + juce::String(angle) + "," + "cap=" + juce::String(getC()) + "</L_CAP>";
    return result;
}

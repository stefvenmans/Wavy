/*
  ==============================================================================

    Resistor.cpp
    Created: 29 Apr 2021 4:45:28pm
    Author:  Stef

  ==============================================================================
*/

#include "Resistor.h"

Resistor::Resistor() : OnePortComponent("res.svg")
{
    R = 1;
}

wdfTreeNode* Resistor::createWDFComponent() {
    treeNode.reset(new wdfTerminatedRes(R));
    return treeNode.get();
}

ComponentType Resistor::getComponentType() {
    return ComponentType::L_RES;
}

void Resistor::setR(double newR){
    R = newR;
}

double Resistor::getR(){
    return R;
}

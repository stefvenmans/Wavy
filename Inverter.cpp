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


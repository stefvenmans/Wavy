/*
  ==============================================================================

    VoltageSource.cpp
    Created: 29 Apr 2021 4:45:58pm
    Author:  Stef

  ==============================================================================
*/

#include "VoltageSource.h"

VoltageSource::VoltageSource() : OnePortComponent("vol.svg")
{
    Vs = 1;
    Rs = 1;
}

wdfTreeNode* VoltageSource::createWDFComponent() {
    treeNode.reset(new wdfTerminatedResVSource(Vs,Rs));
    return treeNode.get();
}

ComponentType VoltageSource::getComponentType() {
    return ComponentType::L_VOL;
}

void VoltageSource::setVs(double newVs){
    Vs = newVs;
}

double VoltageSource::getVs(){
    return Vs;
}

void VoltageSource::setRs(double newRs){
    Rs = newRs;
}

double VoltageSource::getRs(){
    return Rs;
}


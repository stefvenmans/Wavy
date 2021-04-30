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

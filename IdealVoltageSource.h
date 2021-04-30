/*
  ==============================================================================

    IdealVoltageSource.h
    Created: 29 Apr 2021 4:48:50pm
    Author:  Stef

  ==============================================================================
*/

#pragma once
#include "SimpleRootComponent.h"

class IdealVoltageSource : public SimpleRootComponent
{
public:
    IdealVoltageSource();
    wdfRootNode * createWDFComponent() override;
    ComponentType getComponentType() override;
    void setVs(double newVs);
    double getVs();
    
private:
    double Vs;
};

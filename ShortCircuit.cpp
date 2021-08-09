/*
  ==============================================================================

    ShortCircuit.cpp
    Created: 29 Apr 2021 4:49:07pm
    Author:  Stef

  ==============================================================================
*/

#include "ShortCircuit.h"

ShortCircuit::ShortCircuit() : SimpleRootComponent("shortcir.svg")
{
    
}

wdfRootNode * ShortCircuit::createWDFComponent() {
    if(child != nullptr){
        root.reset(new wdfShortCircuit() );
    }
    else return nullptr;
    return root.get();
}

ComponentType ShortCircuit::getComponentType() {
    return ComponentType::SR_SHC;
}

juce::String ShortCircuit::getInfo(){
    juce::String result;
    result = juce::String("<") + ComponentTypeString[getComponentType()] +">" + "name=" + getName() + "," + "x="  +  juce::String(getX()) + "," + "y=" + juce::String(getY()) + "," + "ang=" + juce::String(angle);
    result += "</"+ComponentTypeString[getComponentType()]+">";
    return result;
}

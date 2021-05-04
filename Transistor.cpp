/*
  ==============================================================================

    Transistor.cpp
    Created: 2 May 2021 8:52:18pm
    Author:  Stef

  ==============================================================================
*/

#include "Transistor.h"

Transistor::Transistor() : NonLinearComponent("")
{
    auto svgFile = juce::File::getCurrentWorkingDirectory().getChildFile("transistor.svg");
    svgDrawable = juce::Drawable::createFromSVGFile(svgFile);
    if (svgDrawable != nullptr)
    {
        if (auto svgDrawableComposite = dynamic_cast<juce::DrawableComposite*> (svgDrawable.get())){
            svgDrawableComposite->setBoundingBox ({ static_cast<float>(getX()+10), static_cast<float>(getY()+10), 180.0f, 80.0f });
        }
    }
}

void Transistor::paint (juce::Graphics& g)
{
    
    svgDrawable->draw (g, getAlpha(), getTransform());
    
}

int Transistor::getCollums(){
    return 2;
}

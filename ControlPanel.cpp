/*
  ==============================================================================

    ControlPanel.cpp
    Created: 2 May 2021 10:53:47am
    Author:  Stef

  ==============================================================================
*/

#include "ControlPanel.h"

ControlPanel::ControlPanel(){
    std::cout << juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentApplicationFile).getFullPathName() << std::endl;
    auto svgFile = appDataPath.getChildFile("playbutton.svg");
    svgDrawable = juce::Drawable::createFromSVGFile(svgFile);
    if (svgDrawable != nullptr)
    {
        if (auto svgDrawableComposite = dynamic_cast<juce::DrawableComposite*> (svgDrawable.get())){
            svgDrawableComposite->setBoundingBox ({ static_cast<float>(getX()+5), static_cast<float>(getY()+5), 20,20 });
        }
    }
    
}

void ControlPanel::paint (juce::Graphics& g){
    g.setColour(juce::Colours::darkgrey);
    g.fillAll();
    //svgDrawable->draw (g, getAlpha(),getTransform());
    //std::cout << "All good till here CP" << std::endl;
}

/*
  ==============================================================================

    YParameterAmp.cpp
    Created: 29 Apr 2021 4:47:16pm
    Author:  Stef

  ==============================================================================
*/

#include "YParameterAmp.h"

YParameterAmp::YParameterAmp() : TwoPortComponent("y-par-amp.svg")
{
    auto svgFile = juce::File::getCurrentWorkingDirectory().getChildFile(svgFileName);
    svgDrawable = juce::Drawable::createFromSVGFile(svgFile);
    if (svgDrawable != nullptr)
    {
        if (auto svgDrawableComposite = dynamic_cast<juce::DrawableComposite*> (svgDrawable.get())){
            svgDrawableComposite->setBoundingBox ({ static_cast<float>(getX()+10), static_cast<float>(getY()+10), 180.0f, 80.0f });
        }
    }
}

wdfTreeNode* YParameterAmp::createWDFComponent() {
    treeNode.reset(new wdfInverter(child->createWDFComponent()));
    return treeNode.get();
}

ComponentType YParameterAmp::getComponentType() {
    return ComponentType::A_INV;
}

void YParameterAmp::paint(juce::Graphics& g)
{
    if(rotate){
        angle += juce::MathConstants<float>::halfPi;
        if(angle >= juce::MathConstants<float>::twoPi) angle = 0;
        rotate = false;
        this->setSize(getHeight(),getWidth());
        auto svgFile = juce::File::getCurrentWorkingDirectory().getChildFile(svgFileName);
        svgDrawable = juce::Drawable::createFromSVGFile(svgFile);
        if (svgDrawable != nullptr)
        {
            if (auto svgDrawableComposite = dynamic_cast<juce::DrawableComposite*> (svgDrawable.get())){
                svgDrawableComposite->setBoundingBox ({ static_cast<float>(getX()+10), static_cast<float>(getY()+10), static_cast<float>(getWidth()-20), static_cast<float>(getHeight()-20) });
            }
        }
    }
    
    svgDrawable->draw (g, getAlpha(),getTransform().rotated(angle, getWidth()/2, getHeight()/2));
    std::cout << "paint called" << std::endl;
}


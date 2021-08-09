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
    portOrientations.push_back(2);
    isConnected.push_back(false);
}

void Transistor::paint (juce::Graphics& g)
{
    
    svgDrawable->draw (g, getAlpha(), getTransform());
    
}

int Transistor::getCollums(){
    return 2;
}

int Transistor::connect(CircuitComponent* c) {
    //Check if at right side
    auto index = 0;
    bool connectSuccesfull = false;
    for(auto &o : portOrientations){
        switch(o){
            case 0:
                //Check if component is above + has orientation 2
                if(c->getY() + c->getHeight() == getY() && c->hasOrientation(2)){
                    std::cout << "circuit will be able to connect to this side" << std::endl;
                    connectSuccesfull = true;
                }
                break;
            case 1:
                //Check if component is right + has orientation 3
                if(getX() + getWidth() == c->getX() && c->hasOrientation(3)){
                    std::cout << "circuit will be able to connect to this side" << std::endl;
                    connectSuccesfull = true;
                }
                break;
            case 2:
                //Check if component is under + has orientation 0
                if(getY() + getHeight() == c->getY() && c->hasOrientation(0)){
                    std::cout << "circuit will be able to connect to this side" << std::endl;
                    connectSuccesfull = true;
                }
                break;
            case 3:
                //Check if component is left + has orientation 1
                if(c->getX() + c->getWidth() == getX() && c->hasOrientation(1)){
                    std::cout << "circuit will be able to connect to this side" << std::endl;
                    connectSuccesfull = true;
                }
                break;
        }
        if(connectSuccesfull){
            isConnected[index] = true;
            
            return 1;
        }
        index++;
    }
    return -1;
}

ComponentType Transistor::getComponentType(){
    return NL_TRAN;
}

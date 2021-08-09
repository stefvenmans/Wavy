/*
  ==============================================================================

    TwoPortComponent.cpp
    Created: 29 Apr 2021 4:46:38pm
    Author:  Stef

  ==============================================================================
*/

#include "TwoPortComponent.h"

TwoPortComponent::TwoPortComponent(juce::String svgFileName) : AdaptedLeafComponent(svgFileName), child(nullptr)
{
    portLines.push_back(juce::Line<float>(10,80, 0, 80));
    portLines.push_back(juce::Line<float>(0,20, 10.0f, 20));
    portLines.push_back(juce::Line<float>(100,80, 90, 80));
    portLines.push_back(juce::Line<float>(90,20, 90+ 10.0f, 20));
    
    portOrientations.push_back(1);
    portOrientations.push_back(3);
    
    isConnected.push_back(false);
    isConnected.push_back(false);
    
    isAdapted.push_back(false);
    isAdapted.push_back(false);
}

void TwoPortComponent::paint(juce::Graphics& g)
{
    paintSVG(g);
    AdaptedLeafComponent::paint(g);

    g.setColour(juce::Colours::black);
    g.drawText(getName(), 20, 20, 40, 15, juce::Justification::left);
}

void TwoPortComponent::connectChild(AdaptedLeafComponent* newChild){
    child = newChild;
}

void TwoPortComponent::disconnectChild(){
    child = nullptr;
}

int TwoPortComponent::connect(CircuitComponent* c)  {
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
            if(index==0){
                child = (AdaptedLeafComponent*)c;
                std::cout << "child set" << std::endl;
            }
            if(index == 1){
                isAdapted[1] = true;
            }
            return 1;
        }
        index++;
    }
    return -1;
}
    

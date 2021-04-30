/*
  ==============================================================================

    OnePortComponent.cpp
    Created: 29 Apr 2021 4:46:26pm
    Author:  Stef

  ==============================================================================
*/

#include "OnePortComponent.h"

OnePortComponent::OnePortComponent(juce::String svgFileName) : AdaptedLeafComponent(svgFileName)
{
    portOrientations.push_back(1);
    isConnected.push_back(false);
}
 
void OnePortComponent::paint(juce::Graphics& g)
{
    paintSVG(g);
    if(isAdapted){
        wBLine = juce::Line<float>(80,20, 90+ 10.0f, 20);
    }
    else{
        wBLine = juce::Line<float>(90,20, 90+ 10.0f, 20);
    }
    auto wALine = juce::Line<float>(100,80, 90, 80);
    auto wBAdapted = juce::Line<float>(80,15,80,25);
    
    wBLine.applyTransform(getTransform().rotated(angle, getWidth()/2, getHeight()/2));
    wALine.applyTransform(getTransform().rotated(angle, getWidth()/2, getHeight()/2));
    wBAdapted.applyTransform(getTransform().rotated(angle, getWidth()/2, getHeight()/2));
    
    g.drawArrow(wBLine, 1.5, 10,4);
    g.drawArrow(wALine, 1.5, 10,4);
    g.drawLine(wBAdapted,1.5);
    
    for(auto c : isConnected){
        if(c){
            g.setColour(juce::Colours::red);
            g.drawRect(0,0,100,100,5);
        }
    }
    
}

void OnePortComponent::connect(CircuitComponent* c) {
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
            return;
        }
        index++;
    }
}
    


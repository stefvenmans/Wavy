/*
  ==============================================================================

    SimpleRootComponent.cpp
    Created: 29 Apr 2021 4:48:37pm
    Author:  Stef

  ==============================================================================
*/

#include "SimpleRootComponent.h"

SimpleRootComponent::SimpleRootComponent(juce::String svgFileName) : CircuitComponent(svgFileName), child(nullptr)
{
    portOrientations.push_back(1);
    isConnected.push_back(false);
}

void SimpleRootComponent::paint(juce::Graphics& g) 
{
    paintSVG(g);
    g.setColour(juce::Colours::black);
    g.drawRect(10,10,componentWidth-20,componentHeight-20,3);
    
    auto index = 0;
    for(auto p: portOrientations){
        juce::Line<float> wA;
        juce::Line<float> wB;
        switch(p){
            case 0:
                if(isConnected[index]){
                    wB = juce::Line<float>(20, 10, 20, 0);
                    g.drawLine(wB,1.5);
                }
                else{
                    wB = juce::Line<float>(20, 10, 20, 0);
                    g.drawArrow(wB,1.5,10,4);
                }
                wA = juce::Line<float>(100-20, 0, 100-20, 10);
                break;
            case 1:
                if(isConnected[index]){
                    wB = juce::Line<float>(100-10, 20, 100, 20);
                    g.drawLine(wB,1.5);
                }
                else{
                    wB = juce::Line<float>(100-10, 20, 100, 20);
                    g.drawArrow(wB,1.5,10,4);
                }
                wA = juce::Line<float>(100, 100-20, 100-10, 100-20);
                
                break;
            case 2:
                if(isConnected[index]){
                    wB = juce::Line<float>(100-20, 100-10, 100-20, 100);
                    g.drawLine(wB,1.5);
                }
                else{
                    wB = juce::Line<float>(100-20, 100-10, 100-20, 100);
                    g.drawArrow(wB,1.5,10,4);
                }
                wA = juce::Line<float>(20,100, 20, 100-10);
                
                break;
            case 3:
                if(isConnected[index]){
                    wB = juce::Line<float>(10, 100-20, 0, 100-20);
                    g.drawLine(wB,1.5);
                }
                else{
                    wB = juce::Line<float>(10, 100-20, 0, 100-20);
                    g.drawArrow(wB,1.5,10,4);
                }
                wA = juce::Line<float>(0, 20, 10, 20);
                
                break;
        }
        g.drawArrow(wA,1.5,10,4);
        
        index++;
    }
    
    
    //std::cout << "port orientation" << portOrientations[0] << std::endl;
}

wdfRootNode * SimpleRootComponent::createWDFComponent() {
    return root.get();
}

int SimpleRootComponent::connect(CircuitComponent* c) {
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
            std::cout << "child set" << std::endl;
            child = (AdaptedLeafComponent*)c;
            return 1;
        }
        index++;
    }
}

AdaptedLeafComponent* SimpleRootComponent::getChildComponent(){
    return child;
}

int SimpleRootComponent::isRootOrNonLin(){
    return 1;
}

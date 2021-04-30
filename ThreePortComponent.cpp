/*
  ==============================================================================

    ThreePortComponent.cpp
    Created: 29 Apr 2021 4:48:00pm
    Author:  Stef

  ==============================================================================
*/

#include "ThreePortComponent.h"

ThreePortComponent::ThreePortComponent(juce::String svgFileName) : AdaptedLeafComponent(svgFileName), leftChild(nullptr), rightChild(nullptr)
{
    portLines.push_back(juce::Line<float>(10,80, 0, 80));
    portLines.push_back(juce::Line<float>(0,20, 10.0f, 20));
    portLines.push_back(juce::Line<float>(80,0, 80, 10));
    portLines.push_back(juce::Line<float>(20,10, 20, 0));
    portLines.push_back(juce::Line<float>(100,80, 90, 80));
    portLines.push_back(juce::Line<float>(90,20, 90+ 10.0f, 20));
    
    portOrientations.push_back(1);
    portOrientations.push_back(3);
    portOrientations.push_back(0);
    
    isConnected.push_back(false);
    isConnected.push_back(false);
    isConnected.push_back(false);
}

void ThreePortComponent::paint(juce::Graphics& g)
{
    paintSVG(g);
    
    for(auto l : portLines){
        l.applyTransform(getTransform().rotated(angle, getWidth()/2, getHeight()/2));
        g.drawArrow(l, 1.5, 10,4);
       
     }
}

void ThreePortComponent::connectLeftChild(AdaptedLeafComponent* newLeftChild){
    leftChild = newLeftChild;
}

void ThreePortComponent::connectRightChild(AdaptedLeafComponent* newRightChild){
    rightChild = newRightChild;
}

void ThreePortComponent::disconnectLeftChild(){
    leftChild = nullptr;
}

void ThreePortComponent::disconnectRightChild(){
    rightChild = nullptr;
}

void ThreePortComponent::connect(CircuitComponent* c)  {
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
                rightChild = (AdaptedLeafComponent*)c;
                std::cout << "right child set" << std::endl;
            }
            else if(index==1){
                leftChild = (AdaptedLeafComponent*)c;
                std::cout << "left child set" << std::endl;
            }
            return;
        }
        index++;
    }
}

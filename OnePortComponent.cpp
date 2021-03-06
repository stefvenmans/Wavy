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
    portArrowsA.push_back(juce::Line<float>(100,80, 90, 80));
    portArrowsB.push_back(juce::Line<float>(90,20, 90+ 10.0f, 20));
    isAdapted.push_back(false);
    
}
 
void OnePortComponent::paint(juce::Graphics& g)
{
    paintSVG(g);
    AdaptedLeafComponent::paint(g);

    g.setColour(juce::Colours::black);
    std::vector<int>::iterator it = std::find(portOrientations.begin(), portOrientations.end(), 0);
    if (it != portOrientations.end()){
        g.drawText(getName(), 20, 100 - 20 - 15, 40, 15, juce::Justification::left);
    }
    else{
        g.drawText(getName(), 20, 20, 40, 15, juce::Justification::left);
    }
    
}

int OnePortComponent::connect(CircuitComponent* c) {
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
            isAdapted[index] = true;
            return 1;
        }
        index++;
    }
    return -1;
}
    


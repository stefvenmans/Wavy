/*
  ==============================================================================

    AdaptedLeafComponent.cpp
    Created: 29 Apr 2021 4:45:06pm
    Author:  Stef

  ==============================================================================
*/

#include "AdaptedLeafComponent.h"

    AdaptedLeafComponent::AdaptedLeafComponent(juce::String svgFileName) : CircuitComponent(svgFileName)
    {
        
        
    }

    void AdaptedLeafComponent::paint(juce::Graphics& g)
    {
        auto index = 0;
        for(auto p: portOrientations){
            juce::Line<float> wA;
            juce::Line<float> wB;
            juce::Line<float> adaptBar;
            switch(p){
                case 0:
                    if(isConnected[index]){
                        if(isAdapted[index]){
                            wB = juce::Line<float>(20, 20, 20, 0);
                            adaptBar = juce::Line<float>(15,20,25,20);
                        }
                        else{
                            wB = juce::Line<float>(20, 10, 20, 0);
                        }
                        g.drawLine(wB,1.5);
                        g.drawLine(adaptBar,1.5);
                    }
                    else{
                        wB = juce::Line<float>(20, 10, 20, 0);
                        g.drawArrow(wB,1.5,10,4);
                    }
                    wA = juce::Line<float>(100-20, 0, 100-20, 10);
                    break;
                case 1:
                    if(isConnected[index]){
                        if(isAdapted[index]){
                            wB = juce::Line<float>(100-20, 20, 100, 20);
                            adaptBar = juce::Line<float>(80,15,80,25);
                        }
                        else{
                            wB = juce::Line<float>(100-10, 20, 100, 20);
                        }
                        g.drawLine(wB,1.5);
                        g.drawLine(adaptBar,1.5);
                    }
                    else{
                        wB = juce::Line<float>(100-10, 20, 100, 20);
                        g.drawArrow(wB,1.5,10,4);
                    }
                    wA = juce::Line<float>(100, 100-20, 100-10, 100-20);
                    
                    break;
                case 2:
                    if(isConnected[index]){
                        if(isAdapted[index]){
                            wB = juce::Line<float>(100-20, 100-20, 100-20, 100);
                            adaptBar = juce::Line<float>(75,80,85,80);
                        }
                        else{
                            wB = juce::Line<float>(100-20, 100-10, 100-20, 100);
                        }
                        g.drawLine(wB,1.5);
                        g.drawLine(adaptBar,1.5);
                    }
                    else{
                        wB = juce::Line<float>(100-20, 100-10, 100-20, 100);
                        g.drawArrow(wB,1.5,10,4);
                    }
                    wA = juce::Line<float>(20,100, 20, 100-10);
                    
                    break;
                case 3:
                    if(isConnected[index]){
                        if(isAdapted[index]){
                            wB = juce::Line<float>(20, 100-20, 0, 100-20);
                            adaptBar = juce::Line<float>(20,75,20,85);
                        }
                        else{
                            wB = juce::Line<float>(10, 100-20, 0, 100-20);
                        }
                        g.drawLine(wB,1.5);
                        g.drawLine(adaptBar,1.5);
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
    }
    
    wdfTreeNode* AdaptedLeafComponent::createWDFComponent(){
        return treeNode.get();
    }

    wdfTreeNode* AdaptedLeafComponent::getWDFComponent(){
        return treeNode.get();
    }
  

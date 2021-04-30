/*
  ==============================================================================

    Capacitor.cpp
    Created: 29 Apr 2021 4:45:38pm
    Author:  Stef

  ==============================================================================
*/

#include "CircuitComponent.h"


CircuitComponent::CircuitComponent(juce::String svgFileName) : svgFileName{svgFileName}
{
    auto svgFile = juce::File::getCurrentWorkingDirectory().getChildFile(svgFileName);
    svgDrawable = juce::Drawable::createFromSVGFile(svgFile);
    if (svgDrawable != nullptr)
    {
        if (auto svgDrawableComposite = dynamic_cast<juce::DrawableComposite*> (svgDrawable.get())){
            svgDrawableComposite->setBoundingBox ({ static_cast<float>(getX()+10), static_cast<float>(getY()+10), 80.0f, 80.0f });
        }
    }
}

void CircuitComponent::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::white);
    if(rotate){
        angle += juce::MathConstants<float>::halfPi;
        if(angle >= juce::MathConstants<float>::twoPi) angle = 0;
        rotate = false;
    }
    svgDrawable->draw (g, getAlpha(), getTransform().rotated(angle, getWidth()/2, getHeight()/2));
    
}

void CircuitComponent::paintSVG(juce::Graphics& g)
{
    if(rotate){
        angle += juce::MathConstants<float>::halfPi;
        if(angle >= juce::MathConstants<float>::twoPi) angle = 0;
        rotate = false;
    }
    
    svgDrawable->draw (g, getAlpha(), getTransform().rotated(angle, getWidth()/2, getHeight()/2));
}

void CircuitComponent::resized()
{
    constrainer.setMinimumOnscreenAmounts (getHeight(), getWidth(),
    getHeight(), getWidth());
}

void CircuitComponent::mouseDown(const juce::MouseEvent& e)
{
    lastX = getX();
    lastY = getY();
    if(e.mods.isCommandDown() && e.mods.isShiftDown()){
        rotate = true;
        shooldDrag = false;
        repaint();
        
        for(auto &o: portOrientations){
            o++;
            if(o > 3) o = 0;
        }
        
    }
    else if(e.mods.isCommandDown()){
        shooldDrag = false;
        if(propertyPanelCallback != nullptr){
            propertyPanelCallback(this);
        }
    }
    
    else dragger.startDraggingComponent (this, e);
}

void CircuitComponent::mouseDrag(const juce::MouseEvent& e)
{
    if(shooldDrag == true) dragger.dragComponent (this, e, &constrainer);
    // should check if component moved
    if(getX()!=lastX || getY()!=lastY){
        for(auto c: isConnected){
            c = false;
        }
        // get all child components and diconnect them
        //std::cout << "disconnect all" << std::endl;
        repaint();
    }
    
}

void CircuitComponent::mouseUp(const juce::MouseEvent& e)
{
    const int width = getWidth();
    const int height = getHeight();
    const int x = getX();
    const int y = getY();
    if(x % width >= width/2){
        if(getY() % height >= height/2){
            setBounds(x + width - x % width, y + height - y % height, width, height);
        }
        else{
            setBounds(x + width - x % width, y - y % height, width, height);
        }
    }
    else{
        if(getY() % height >= height/2){
            setBounds(x - x % width, y + height - y % height, width, height);
        }
        else{
            setBounds(x - x % width, y - y % height, width, height);
        }
    }
    shooldDrag = true;
    
    
    
    if(callBack != NULL){
        if(callBack(this)){
            //isConnected = true;
        }
    }
    
    
}

void CircuitComponent::addHandler(std::function<bool(CircuitComponent* c)> clbk)
{
    callBack = clbk;
}

float CircuitComponent::getRotationX()
{
    return std::cos(-angle);
}

float CircuitComponent::getRotationY()
{
    return std::sin(angle);
}

void CircuitComponent::connect(CircuitComponent* c){
    
}

bool CircuitComponent::hasOrientation(int orientation){
    for(auto &o: portOrientations){
        if(o == orientation) return true;
    }
    return false;
}

ComponentType CircuitComponent::getComponentType(){
    return ComponentType::NOT_SET;
}

void CircuitComponent::setPropertyPanelCallback(std::function<void(CircuitComponent* c)> callbackFunction){
    propertyPanelCallback = callbackFunction;
}
    
int CircuitComponent::getCollums(){
    return 1;
}

int CircuitComponent::getRows(){
    return 1;
}

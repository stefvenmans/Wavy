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
    auto svgFile = appDataPath.getChildFile(svgFileName);
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
    //g.fillAll(juce::Colours::white);
    if(rotate){
        angle += juce::MathConstants<float>::halfPi;
        if(angle >= juce::MathConstants<float>::twoPi) angle = 0;
        rotate = false;
    }
    
    if(componentIsSelected){
        g.setColour(juce::Colour((juce::uint8)0, (juce::uint8)0, (juce::uint8)80, (juce::uint8)50));
        g.fillRect(10, 10, 80, 80);
        std::cout << "dbg" << std::endl;
    }
    
    svgDrawable->draw (g, getAlpha(), getTransform().rotated(angle, getWidth()/2, getHeight()/2));
    
}

void CircuitComponent::paintSVG(juce::Graphics& g)
{
    if(componentIsSelected){
        g.setColour(juce::Colour((juce::uint8)0, (juce::uint8)10, (juce::uint8)50, (juce::uint8)20));
        g.fillRect(10, 10, 80, 80);
        g.setColour(juce::Colours::black);
    }
    
    svgDrawable->draw (g, getAlpha(), getTransform().rotated(angle, getWidth()/2, getHeight()/2));
}

void CircuitComponent::resized()
{
    constrainer.setMinimumOnscreenAmounts (getHeight(), getWidth(),
    getHeight(), getWidth());
}

int CircuitComponent::getCollums(){
    return 1;
}

int CircuitComponent::getRows(){
    return 1;
}

void CircuitComponent::rotateComponent(){
    //rotate = true;
    shooldDrag = false;
    
    angle += juce::MathConstants<float>::halfPi;
    if(angle >= juce::MathConstants<float>::twoPi) angle = 0;
    
    for(auto &o: portOrientations){
        o++;
        if(o > 3) o = 0;
    }
    repaint();
}

void CircuitComponent::mouseDown(const juce::MouseEvent& e)
{
    lastX = getX();
    lastY = getY();
    if(e.mods.isCommandDown() && e.mods.isShiftDown()){
        rotateComponent();
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
    const int width = componentWidth;
    const int height = componentHeight;
    const int x = getX();
    const int y = getY();
    if(x % width >= width/2){
        if(getY() % height >= height/2){
            setBounds(x + width - x % width, y + height - y % height, width*getCollums(), height*getRows());
        }
        else{
            setBounds(x + width - x % width, y - y % height, width*getCollums(), height*getRows());
        }
    }
    else{
        if(getY() % height >= height/2){
            setBounds(x - x % width, y + height - y % height, width*getCollums(), height*getRows());
        }
        else{
            setBounds(x - x % width, y - y % height, width*getCollums(), height*getRows());
            
        }
    }
    if(x == getX() && y == getY()) {
        if(componentIsSelected!=true){
            componentIsSelected = true;
            repaint();
            if(componentIsSelectedCallBack != nullptr) componentIsSelectedCallBack(this);
            if(propertyPanelCallback != nullptr){
                propertyPanelCallback(this);
            }
        }
    }
    shooldDrag = true;
    
    
    
    if(callBack != NULL){
        if(callBack(this)){
            //isConnected = true;
        }
    }
    
    
}

void CircuitComponent::mouseDoubleClick(const juce::MouseEvent &e){
    std::cout << "mouse double clicked" << std::endl;
    if(propertyPanelCallback != nullptr){
        propertyPanelCallback(this);
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

int CircuitComponent::connect(CircuitComponent* c){
    
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
    
juce::String CircuitComponent::getName(){
    return componentName;
}

void CircuitComponent::setName(juce::String name){
    componentName = name;
    repaint();
}


//0 for leaf, 1 for simple root, 2 for non-lin root
int CircuitComponent::isRootOrNonLin(){
    return -1;
}

void CircuitComponent::setControl(juce::Slider* s){
    slider = s;
}

juce::Slider* CircuitComponent::getControl(){
    return slider;
}

juce::String CircuitComponent::getInfo(){
    return "CircuitComponent";
}

bool CircuitComponent::isComponentConnected(){
//    int i=0;
//    for(auto b : isConnected){
//        if(b) i+=1;
//    }
//    if (i==isConnected.size()) return true;
//    else return false;
    if(isConnected.back()) return true;
    else return false;
}

void CircuitComponent::unSelectComponent(){
    componentIsSelected = false;
    const juce::MessageManagerLock mmLock;
    repaint();
}

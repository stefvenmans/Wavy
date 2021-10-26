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
            svgDrawableComposite->setBoundingBox ({ static_cast<float>(getX()+componentOffset), static_cast<float>(getY()+componentOffset), static_cast<float>(componentWidth-2*componentOffset), static_cast<float>(componentHeight-2*componentOffset) });
        }
    }
}

void CircuitComponent::paint (juce::Graphics& g)
{
    if(componentIsSelected){
        g.setColour(juce::Colour((juce::uint8)0, (juce::uint8)0, (juce::uint8)(80), (juce::uint8)50));
        g.fillRect(10, 10, componentWidth-2*componentOffset, componentHeight-2*componentOffset);
    }
    
    svgDrawable->draw (g, getAlpha(), getTransform().rotated(angle, getWidth()/2, getHeight()/2));
    
}

void CircuitComponent::paintSVG(juce::Graphics& g)
{
    if(componentIsSelected){
        g.setColour(juce::Colour((juce::uint8)0, (juce::uint8)10, (juce::uint8)50, (juce::uint8)20));
        g.fillRect(10, 10, componentWidth-2*componentOffset, componentHeight-2*componentOffset);
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
    
    //Rotate
    if(e.mods.isCommandDown() && e.mods.isShiftDown()){
        rotateComponent();
        rotate = true;
    }
    
    //Open property panel
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
    
    //Disconnect component if moved. TODO: disconnect all neighbour components
    if(getX()!=lastX || getY()!=lastY){
        for(auto c: isConnected){
            c = false;
        }
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
    if(x == getX() && y == getY() && rotate != true) {
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
    
    //See if this component can connect to other components around it.
    if(wantToConnectCallback != NULL){
        if(wantToConnectCallback(this)){
            //isConnected = true;
        }
    }
    rotate = false;
}

void CircuitComponent::mouseDoubleClick(const juce::MouseEvent &e){
    if(propertyPanelCallback != nullptr && rotate!=true){
        propertyPanelCallback(this);
    }
}

void CircuitComponent::addHandler(std::function<bool(CircuitComponent* c)> clbk)
{
    wantToConnectCallback = clbk;
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
    return -1;
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

void CircuitComponent::setPropertyPanelCallback(std::function<void(CircuitComponent* c)> clbk){
    propertyPanelCallback = clbk;
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
    juce::String result;
    result = juce::String("<") + ComponentTypeString[getComponentType()] +">" + "name=" + getName() + "," + "x="  +  juce::String(getX()) + "," + "y=" + juce::String(getY()) + "," + "ang=" + juce::String(angle);
    result += "</"+ComponentTypeString[getComponentType()]+">";
    return result;
}

bool CircuitComponent::isComponentConnected(){
    if(isConnected.back()) return true;
    else return false;
}

void CircuitComponent::unSelectComponent(){
    componentIsSelected = false;
    const juce::MessageManagerLock mmLock;
    repaint();
}

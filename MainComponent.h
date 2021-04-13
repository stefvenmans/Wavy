#pragma once

#include <JuceHeader.h>
#include <functional>
#include <stdlib.h>
#include "../rt-wdf_lib/Libs/rt-wdf/rt-wdf.h"

class WdfEnvironment : public wdfTree
{
public:
    WdfEnvironment(){
        subtreeCount = 1;
        subtreeEntryNodes = new wdfTreeNode*[subtreeCount];
    }
    void setInputValue( double signalIn ){
        
    }
    double getOutputValue( ){
        
    }
    int setRootMatrData( matData* rootMatrixData, double *Rp ){
        
    }
    const char* getTreeIdentifier( ){
        
    }
    void setParam( size_t paramID, double paramValue ){
        
    }
    
    void setSubtreeEntryNodes(wdfTreeNode * node){
        subtreeEntryNodes[0] = node;
    }
    
    void setRoot(wdfRootNode * r){
        root.reset(new wdfRootSimple(r));
        Rp = new double[subtreeCount]();
    }
    
    void addParam(paramData p){
        params.push_back(p);
    }
    
};

class Resistor : public juce::Component
{
public:
    Resistor(){
        auto file = juce::File::getCurrentWorkingDirectory().getChildFile ("res.svg");
        svgSymbol = juce::Drawable::createFromSVGFile(file);
        if (svgSymbol != nullptr)
        {
            if (auto comp = dynamic_cast<juce::DrawableComposite*> (svgSymbol.get()))
                comp->setBoundingBox ({ static_cast<float>(getX()+10), static_cast<float>(getY()+10), 80.0f, 80.0f });
                
        }
        setName("R");
    }
    void paint (juce::Graphics& g) override{
        g.fillAll(juce::Colours::white);
        if(rotate){
            angle += juce::MathConstants<float>::halfPi;
            if(angle >= juce::MathConstants<float>::twoPi) angle = 0;
            rotate = false;
        }
        svgSymbol->draw (g, getAlpha(), getTransform().rotated(angle, getWidth()/2, getHeight()/2));
        
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
        //g.drawLine(line);
        
        
    }
    
    wdfTreeNode * createWDFComponent(double Rp){
        r.reset(new wdfTerminatedRes(Rp));
        return r.get();
    }
    wdfTreeNode * getWDFComponent(){
        return r.get();
    }
    
    void resized() override{
        constrainer.setMinimumOnscreenAmounts (getHeight(), getWidth(),
        getHeight(), getWidth());
        
    }
    
    void mouseDown(const juce::MouseEvent& e) override{
        if(e.mods.isCommandDown() && e.mods.isShiftDown()){
            rotate = true;
            shooldDrag = false;
            repaint();
        }
        else dragger.startDraggingComponent (this, e);

        
    }
    void mouseDrag(const juce::MouseEvent& e) override{
        if(shooldDrag == true) dragger.dragComponent (this, e, &constrainer);
    }
    void mouseUp(const juce::MouseEvent& e) override{
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
    
    void addHandler(std::function<bool(juce::Component* c)> clbk){
        callBack = clbk;
    }
    
    int getRotationX(){
        std::cout << angle << std::endl;
        return -std::cos(angle);
    }
    
    int getRotationY(){
        std::cout << angle << std::endl;
        return -std::sin(angle);
    }
    
private:
    std::unique_ptr<juce::Drawable> svgSymbol;
    std::unique_ptr<wdfTerminatedRes> r;
    bool rotate = false;
    bool shooldDrag = true;
    bool isAdapted = true;
    float angle = 0;
    juce::Line<float> wBLine;
    
    
    juce::ComponentBoundsConstrainer constrainer;
    juce::ComponentDragger dragger;
    
    std::function<bool(Component*c)> callBack;
};

class Capacitor : public juce::Component
{
public:
    Capacitor(){
        auto file = juce::File::getCurrentWorkingDirectory().getChildFile ("cap.svg");
        svgSymbol = juce::Drawable::createFromSVGFile(file);
        if (svgSymbol != nullptr)
        {
            if (auto comp = dynamic_cast<juce::DrawableComposite*> (svgSymbol.get()))
                comp->setBoundingBox ({ static_cast<float>(getX()+10), static_cast<float>(getY()+10), 80.0f, 80.0f });
        }
        setName("C");
    }
    void paint (juce::Graphics& g) override{
        g.fillAll(juce::Colours::white);
        if(rotate){
            angle += juce::MathConstants<float>::halfPi;
            if(angle >= juce::MathConstants<float>::twoPi) angle = 0;
            rotate = false;
        }
        svgSymbol->draw (g, getAlpha(), getTransform().rotated(angle, getWidth()/2, getHeight()/2));
        
        auto wBLine = juce::Line<float>(90,20, 90+ 10.0f, 20);
        auto wALine = juce::Line<float>(100,80, 90, 80);
        wBLine.applyTransform(getTransform().rotated(angle, getWidth()/2, getHeight()/2));
        wALine.applyTransform(getTransform().rotated(angle, getWidth()/2, getHeight()/2));
        g.drawArrow(wBLine, 2, 10,4);
        g.drawArrow(wALine, 2, 10,4);
    }
    
    void resized() override{
        constrainer.setMinimumOnscreenAmounts (getHeight(), getWidth(),
        getHeight(), getWidth());
        
    }
    
    wdfTreeNode * createWDFComponent(double C){
        c.reset(new wdfTerminatedCap(C,1.0));
        return c.get();
    }
    wdfTreeNode * getWDFComponent(){
        return c.get();
    }
    
    void mouseDown(const juce::MouseEvent& e) override{
        if(e.mods.isCommandDown() && e.mods.isShiftDown()){
            rotate = true;
            shooldDrag = false;
            repaint();
        }
        else dragger.startDraggingComponent (this, e);

        
    }
    void mouseDrag(const juce::MouseEvent& e) override{
        if(shooldDrag == true) dragger.dragComponent (this, e, &constrainer);
    }
    void mouseUp(const juce::MouseEvent& e) override{
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
    }
    
private:
    std::unique_ptr<juce::Drawable> svgSymbol;
    std::unique_ptr<wdfTerminatedCap> c;
    bool rotate = false;
    bool shooldDrag = true;
    float angle = 0;
    
    juce::ComponentBoundsConstrainer constrainer;
    juce::ComponentDragger dragger;
};

class Series : public juce::Component
{
public:
    Series(){
        auto file = juce::File::getCurrentWorkingDirectory().getChildFile ("series.svg");
        svgSymbol = juce::Drawable::createFromSVGFile(file);
        if (svgSymbol != nullptr)
        {
            if (auto comp = dynamic_cast<juce::DrawableComposite*> (svgSymbol.get()))
                comp->setBoundingBox ({ static_cast<float>(getX()+10), static_cast<float>(getY()+10), 80.0f, 80.0f });
        }
        setName("Ser");
    }
    void paint (juce::Graphics& g) override{
        g.fillAll(juce::Colours::white);
        if(rotate){
            angle += juce::MathConstants<float>::halfPi;
            if(angle >= juce::MathConstants<float>::twoPi) angle = 0;
            rotate = false;
        }
        svgSymbol->draw (g, getAlpha(), getTransform().rotated(angle, getWidth()/2, getHeight()/2));
        auto wBLine0 = juce::Line<float>(10,80, 0, 80);
        auto wALine0 = juce::Line<float>(0,20, 10.0f, 20);
        
        auto wBLine1 = juce::Line<float>(80,0, 80, 10);
        auto wALine1 = juce::Line<float>(20,10, 20, 0);
        
        auto wBLine2 = juce::Line<float>(100,80, 90, 80);
        auto wALine2 = juce::Line<float>(90,20, 90+ 10.0f, 20);
        
        wBLine0.applyTransform(getTransform().rotated(angle, getWidth()/2, getHeight()/2));
        wALine0.applyTransform(getTransform().rotated(angle, getWidth()/2, getHeight()/2));
        g.drawArrow(wBLine0, 2, 10,4);
        g.drawArrow(wALine0, 2, 10,4);
        
        wBLine1.applyTransform(getTransform().rotated(angle, getWidth()/2, getHeight()/2));
        wALine1.applyTransform(getTransform().rotated(angle, getWidth()/2, getHeight()/2));
        g.drawArrow(wBLine1, 2, 10,4);
        g.drawArrow(wALine1, 2, 10,4);
        
        wBLine2.applyTransform(getTransform().rotated(angle, getWidth()/2, getHeight()/2));
        wALine2.applyTransform(getTransform().rotated(angle, getWidth()/2, getHeight()/2));
        g.drawArrow(wBLine2, 2, 10,4);
        g.drawArrow(wALine2, 2, 10,4);
    }
    
    void resized() override{
        constrainer.setMinimumOnscreenAmounts (getHeight(), getWidth(),
        getHeight(), getWidth());
        
    }
    
    void createWDFComponent(wdfTreeNode * left, wdfTreeNode * right){
        //ser.reset(new wdfTerminatedSeries(left, right));
        ser.reset(new wdfTerminatedSeries(((Resistor*)(children[0]))->getWDFComponent(), ((Resistor*)(children[1]))->getWDFComponent()));
    }
    wdfTreeNode * getWDFComponent(){
        return ser.get();
    }
    
    void mouseDown(const juce::MouseEvent& e) override{
        if(e.mods.isCommandDown() && e.mods.isShiftDown()){
            rotate = true;
            shooldDrag = false;
            repaint();
        }
        else dragger.startDraggingComponent (this, e);

        
    }
    void mouseDrag(const juce::MouseEvent& e) override{
        if(shooldDrag == true) dragger.dragComponent (this, e, &constrainer);
    }
    void mouseUp(const juce::MouseEvent& e) override{
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
    }
    
    void setChild(Component* c){
        if(children.size()<2){
            children.push_back(c);
            if(children.size()==2) createWDFComponent(nullptr, nullptr);
        }
        
    }
    
private:
    std::unique_ptr<juce::Drawable> svgSymbol;
    std::unique_ptr<wdfTerminatedSeries> ser;
    bool rotate = false;
    bool shooldDrag = true;
    float angle = 0;
    
    juce::ComponentBoundsConstrainer constrainer;
    juce::ComponentDragger dragger;
    
    std::vector<Component*> children;
};

class Parallel : public juce::Component
{
public:
    Parallel(){
        auto file = juce::File::getCurrentWorkingDirectory().getChildFile ("series.svg");
        svgSymbol = juce::Drawable::createFromSVGFile(file);
        if (svgSymbol != nullptr)
        {
            if (auto comp = dynamic_cast<juce::DrawableComposite*> (svgSymbol.get()))
                comp->setBoundingBox ({ static_cast<float>(getX()), static_cast<float>(getY()), 100.0f, 100.0f });
        }
        setName("Par");
    }
    void paint (juce::Graphics& g) override{
        g.fillAll(juce::Colours::white);
        if(rotate){
            angle += juce::MathConstants<float>::halfPi;
            if(angle >= juce::MathConstants<float>::twoPi) angle = 0;
            rotate = false;
        }
        svgSymbol->draw (g, getAlpha(), getTransform().rotated(angle, getWidth()/2, getHeight()/2));
    }
    
    void resized() override{
        constrainer.setMinimumOnscreenAmounts (getHeight(), getWidth(),
        getHeight(), getWidth());
        
    }
    
    void createWDFComponent(wdfTreeNode * left, wdfTreeNode * right){
        par.reset(new wdfTerminatedParallel(left, right));
    }
    wdfTreeNode * getWDFComponent(){
        return par.get();
    }
    
    void mouseDown(const juce::MouseEvent& e) override{
        if(e.mods.isCommandDown() && e.mods.isShiftDown()){
            rotate = true;
            shooldDrag = false;
            repaint();
        }
        else dragger.startDraggingComponent (this, e);

        
    }
    void mouseDrag(const juce::MouseEvent& e) override{
        if(shooldDrag == true) dragger.dragComponent (this, e, &constrainer);
    }
    void mouseUp(const juce::MouseEvent& e) override{
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
    }
    
private:
    std::unique_ptr<juce::Drawable> svgSymbol;
    std::unique_ptr<wdfTerminatedParallel> par;
    bool rotate = false;
    bool shooldDrag = true;
    float angle = 0;
    
    juce::ComponentBoundsConstrainer constrainer;
    juce::ComponentDragger dragger;
};

class IdealVSource : public juce::Component
{
public:
    IdealVSource(){
        auto file = juce::File::getCurrentWorkingDirectory().getChildFile ("idealvol.svg");
        svgSymbol = juce::Drawable::createFromSVGFile(file);
        if (svgSymbol != nullptr)
        {
            if (auto comp = dynamic_cast<juce::DrawableComposite*> (svgSymbol.get()))
                comp->setBoundingBox ({ static_cast<float>(getX()), static_cast<float>(getY()), 100.0f, 100.0f });
        }
        setName("Vsource");
    }
    void paint (juce::Graphics& g) override{
        g.fillAll(juce::Colours::white);
        if(rotate){
            angle += juce::MathConstants<float>::halfPi;
            if(angle >= juce::MathConstants<float>::twoPi) angle = 0;
            rotate = false;
        }
        svgSymbol->draw (g, getAlpha(), getTransform().rotated(angle, getWidth()/2, getHeight()/2));
    }
    
    void resized() override{
        constrainer.setMinimumOnscreenAmounts (getHeight(), getWidth(),
        getHeight(), getWidth());
        
    }
    
    void createWDFComponent(double Vs){
        u.reset(new wdfIdealVSource(Vs));
    }
    wdfRootNode * getWDFComponent(){
        return u.get();
    }
    
    void mouseDown(const juce::MouseEvent& e) override{
        if(e.mods.isCommandDown() && e.mods.isShiftDown()){
            rotate = true;
            shooldDrag = false;
            repaint();
        }
        else dragger.startDraggingComponent (this, e);

        
    }
    void mouseDrag(const juce::MouseEvent& e) override{
        if(shooldDrag == true) dragger.dragComponent (this, e, &constrainer);
    }
    void mouseUp(const juce::MouseEvent& e) override{
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
    }
    
private:
    std::unique_ptr<juce::Drawable> svgSymbol;
    std::unique_ptr<wdfIdealVSource> u;
    bool rotate = false;
    bool shooldDrag = true;
    float angle = 0;
    
    juce::ComponentBoundsConstrainer constrainer;
    juce::ComponentDragger dragger;
};



class RNode : public juce::Component
{
public:
    RNode(){
        
    }
    
    void paint (juce::Graphics& g) override{
        auto area = getLocalBounds().reduced (2);

        g.setColour (juce::Colours::orange);
        g.drawRoundedRectangle (area.toFloat(), 10.0f, 2.0f);

        g.setColour (findColour (juce::TextButton::textColourOffId));
        g.drawFittedText ("R1", area, juce::Justification::centred, 1);
        
        if(drawLine == true){
//            auto mousePoint = getMouseXYRelative();
//            endPoint = juce::Point<float>((float)(mousePoint.getX()),(float)mousePoint.getY());
            
            //endPoint = juce::Point<float>(getX(),getY());
            
        }
        g.setColour(juce::Colours::black);
        
        if(startPoint.getX() >= 50){
            startPoint.setX(80);
        }
        else startPoint.setX(20);
        
        if(startPoint.getY() >= 50){
            startPoint.setY(80);
        }
        else startPoint.setY(20);
        
        if(endPoint.getX() >= 50){
            endPoint.setX(80);
        }
        else endPoint.setX(20);
        
        if(endPoint.getY() >= 50){
            endPoint.setY(80);
        }
        else endPoint.setY(20);
        
        
        for(auto i=0; i<linePoints.size()-1 ; i++){
            if(i==0){
                g.drawLine(juce::Line<float>(*linePoints.getFirst(),*linePoints.getUnchecked(1)),1.5);
            }
            else{ g.drawLine(juce::Line<float>(*linePoints.getUnchecked(i),*linePoints.getUnchecked(i+1)),1.5);
            }
        }
        
        //g.drawLine(juce::Line<float>(startPoint, endPoint), 3);
    }
    void resized() override{
        constrainer.setMinimumOnscreenAmounts (getHeight(), getWidth(),
        getHeight(), getWidth());
    }
    
    void mouseDown(const juce::MouseEvent& e) override{
        if(e.mods.isCommandDown()){
            dragger.startDraggingComponent (this, e);
            isDragging = true;
        }
        else{
            drawLine = true;
            startPoint = juce::Point<float>((float)e.getPosition().getX(),(float)e.getPosition().getY());
            
            
            float x = (float)e.getPosition().getX();
            float y = (float)e.getPosition().getY();
            
            if(x>=50){
                if(x>=100){
                    if(x>=150){
                        x = 180;
                    }
                    else x = 120;
                }
                else x = 80;
                
            }
            else x = 20;
            
            if(y>=50){
                if(y>=100){
                    if(y>=150){
                        y = 180;
                    }
                    else y = 120;
                }
                else y = 80;
                
            }
            else y = 20;
            
            if(linePoints.isEmpty()){
                linePoints.add(new juce::Point<float>(x, y));
            }
            linePoints.add(new juce::Point<float>(x, y));
        }
        
    }
    void mouseDrag(const juce::MouseEvent& e) override{
        if(isDragging){
            dragger.dragComponent (this, e, &constrainer);
        }
        else{
            endPoint = juce::Point<float>((float)e.getPosition().getX(),(float)e.getPosition().getY());
            
            float x = (float)e.getPosition().getX();
            float y = (float)e.getPosition().getY();
            
            if(x>=50){
                if(x>=100){
                    if(x>=150){
                        x = 180;
                    }
                    else x = 120;
                }
                else x = 80;
                
            }
            else x = 20;
            
            if(y>=50){
                if(y>=100){
                    if(y>=150){
                        y = 180;
                    }
                    else y = 120;
                }
                else y = 80;
                
            }
            else y = 20;
            
            linePoints.getLast()->setXY(x,y);
            repaint();
        }
    }
    void mouseUp(const juce::MouseEvent& e) override{
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
        drawLine = true;
        isDragging = false;
    }
private:
    juce::ComponentBoundsConstrainer constrainer;
    juce::ComponentDragger dragger;
    bool drawLine = false;
    juce::Point<float> startPoint;
    juce::Point<float> endPoint;
    juce::OwnedArray<juce::Point<float>> linePoints;
    bool isDragging = false;
    
};


class FrontPanel : public juce::Component
{
public:
    FrontPanel(){
        
    }
    void paint (juce::Graphics& g) override{
        g.setColour(juce::Colours::grey);
        g.fillAll(juce::Colours::grey);
    }
    void addNewComponent(juce::Component* c){
        int x = 0;
        ((juce::Slider*)(c))->setSliderStyle(juce::Slider::SliderStyle::Rotary);
        if(components.isEmpty()) x = 15;
        else x += components.getLast()->getX() + 115;
        ((juce::Slider*)(c))->setBounds(x,15,100,100);
        ((juce::Slider*)(c))->setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true,50,20);
        
        addAndMakeVisible(components.add(c));
    }
    void resized() override{
        
    }
    
private:
    juce::OwnedArray<juce::Component> components;
};

class DraggableComp : public juce::Component{
public:
    DraggableComp(){
        
    }

    void paint (juce::Graphics& g) override{
        auto area = getLocalBounds().reduced (2);

        g.setColour (juce::Colours::orange);
        g.drawRoundedRectangle (juce::Rectangle<float>(10,10,80,80), 10.0f, 2.0f);

        g.setColour (findColour (juce::TextButton::textColourOffId));
        g.drawFittedText ("R1", area, juce::Justification::centred, 1);
        
        g.setColour(juce::Colours::black);
        if(isConnected){
            g.drawEllipse(7.5,20,5,5,5);
            g.drawEllipse(7.5,80,5,5,5);
        }
    }
    void resized() override{
        constrainer.setMinimumOnscreenAmounts (getHeight(), getWidth(),
        getHeight(), getWidth());
    }
    void mouseDown(const juce::MouseEvent& e) override{
        dragger.startDraggingComponent (this, e);
    }
    void mouseDrag(const juce::MouseEvent& e) override{
        dragger.dragComponent (this, e, &constrainer);
        isConnected = false;
    }
    void mouseUp(const juce::MouseEvent& e) override{
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
        
        if(callBack != NULL){
            if(callBack(getX(),getY())){
                isConnected = true;
            }
        }
        
    
        
    }
    
    void addHandler(std::function<bool(int,int)> clbk){
        callBack = clbk;
    }
    
private:
    juce::ComponentBoundsConstrainer constrainer;
    juce::ComponentDragger dragger;
    //bool (*callback)(int,int) = NULL;
    std::function<bool(int,int)> callBack;
    bool isConnected = false;
    
    
};





class Schematic : public juce::Component
{
public:
    Schematic(){
        l1.setText("Dummy text",juce::NotificationType::dontSendNotification);
        addAndMakeVisible(l1);
        l1.setBounds((820*5)/2,(580*5)/2,500,80);
        
        addAndMakeVisible(dragComp);
        dragComp.setBounds((820*5)/2+1000,(580*5)/2, 100,100);
        
        
        
        
    }
    
    void paint (juce::Graphics& g) override{
        g.setColour(juce::Colours::grey);
        g.fillAll(juce::Colours::lightgrey);
        g.drawRect(juce::Rectangle<float>(5000/2-300,5000/2,50,50),15);
        
        
        float x = 1000;
        float y = 1000;
        juce::Path sineGraph;
        
        sineGraph.startNewSubPath(x, y);
    
        auto numberOfPoints = 1000;
        for (int i = 1; i < numberOfPoints; ++i)
        {
     // calculate or optain the coordinates x and y of the current point
            
            x += 0.1;
            y = 1000.0+100.0*std::sin(x*(juce::MathConstants<double>::pi*2.0)/100.0);
            sineGraph.lineTo(x, y);
        }

        float lineThickness = 0.75f;
        g.strokePath (sineGraph, juce::PathStrokeType(lineThickness));
    }
private:
    juce::Label l1;
    DraggableComp dragComp;
};


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;
    
    //Component* getComponentToConnect(Component* child);
    bool isConnectable(int x, int y);
    bool wantsToConnect(juce::Component* c);
    
    static void printSomething(){
        std::cout << "hello world" << std::endl;
    }
    
    void mouseMagnify (const juce::MouseEvent &event, float scaleFactor) override;
    
    
private:
    //==============================================================================
    // Your private member variables go here...
    Resistor r1;
    Resistor r2;
    Series s1;
    IdealVSource v1;
    WdfEnvironment wdfEnvironment;
    
    FrontPanel frontPanel;
    Schematic schematic;
    
    Capacitor c1;
    Parallel p1;
    //DraggableComp draggableComp;
    juce::OwnedArray<juce::Component> components;
    
    juce::TextButton textButton;
    juce::Slider res1Slider;
    juce::ComboBox componentSelector;
    juce::TextButton showLibraryButton;
    
    juce::Viewport viewPort;
    //juce::BlockField blockField;
    
    
    float scale = 1.0f;
    paramData res1Val;
    
    juce::SidePanel sidePanel{"Library",200,true,nullptr,false};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};



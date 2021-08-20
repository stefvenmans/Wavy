/*
  ==============================================================================

    Capacitor.h
    Created: 29 Apr 2021 4:45:38pm
    Author:  Stef

  ==============================================================================
*/

#pragma once
#include "Config.h"

class CircuitComponent : public juce::Component
{
public:
    CircuitComponent(juce::String svgFileName);
    void paint (juce::Graphics& g) override;
    void paintSVG(juce::Graphics& g);
    void resized() override;
    virtual int getCollums();
    virtual int getRows();
    virtual void mouseDown(const juce::MouseEvent& e) override;
    virtual void mouseDoubleClick(const juce::MouseEvent &e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;
    void addHandler(std::function<bool(CircuitComponent* c)> clbk);
    float getRotationX();
    float getRotationY();
    virtual int connect(CircuitComponent* c);
    bool hasOrientation(int orientation);
    virtual ComponentType getComponentType();
    void setPropertyPanelCallback(std::function<void(CircuitComponent* c)> callbackFunction);
    juce::String getName();
    void setName(juce::String);
    virtual int isRootOrNonLin();
    void setControl(juce::Slider* s);
    juce::Slider* getControl();
    virtual juce::String getInfo();
    bool isComponentConnected();
    void rotateComponent();
    std::function<void(CircuitComponent*c)> componentIsSelectedCallBack;
    void unSelectComponent();
    
    
protected:
    juce::String svgFileName;
    std::unique_ptr<juce::Drawable> svgDrawable;
    juce::String componentName;
    juce::ComponentBoundsConstrainer constrainer;
    juce::ComponentDragger dragger;
    bool rotate = false;
    bool shooldDrag = true;
    float angle = 0;
    std::function<bool(CircuitComponent* c)> wantToConnectCallback;
    std::function<void(CircuitComponent* c)> propertyPanelCallback;
    std::vector<int> portOrientations;
    std::vector<bool> isConnected;
    int sizeX;
    int sizeY;
    int lastX;
    int lastY;
    juce::Slider* slider;
    bool componentIsSelected = false;
    
};

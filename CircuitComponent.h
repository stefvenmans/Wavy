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
    virtual void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;
    void addHandler(std::function<bool(CircuitComponent* c)> clbk);
    float getRotationX();
    float getRotationY();
    virtual void connect(CircuitComponent* c);
    bool hasOrientation(int orientation);
    virtual ComponentType getComponentType();
    void setPropertyPanelCallback(std::function<void(CircuitComponent* c)> callbackFunction);
    virtual int getCollums();
    virtual int getRows();
    
protected:
    juce::String svgFileName;
    std::unique_ptr<juce::Drawable> svgDrawable;
    juce::ComponentBoundsConstrainer constrainer;
    juce::ComponentDragger dragger;
    bool rotate = false;
    bool shooldDrag = true;
    float angle = 0;
    std::function<bool(CircuitComponent* c)> callBack;
    std::function<void(CircuitComponent* c)> propertyPanelCallback;
    std::vector<int> portOrientations;
    int sizeX;
    int sizeY;
    std::vector<bool> isConnected;
    int lastX;
    int lastY;
    
};

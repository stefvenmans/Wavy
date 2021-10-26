/*
  ==============================================================================

    FrontPanel.h
    Created: 29 Apr 2021 4:52:46pm
    Author:  Stef

  ==============================================================================
*/

#pragma once
#include "Config.h"
#include "CircuitComponent.h"

class FrontPanel : public juce::Component
{
public:
    FrontPanel();
    void paint (juce::Graphics& g) override;
    void addNewComponent(juce::Component* c);
    void addNewComponent(juce::Component* juceComp, std::function<void()> callback);
    void resized() override;
    void removeComponent(juce::Component* c);
    void plotSignal(bool shouldPlot);
    void addSignalToPlot(std::vector<double> signalIn, std::vector<double> signalOut);
    std::function<void(int)> setSignalType;
    std::function<void()> setSignalAmp;
    std::function<void(double)> setSignalFreq;
    juce::String getOutputFileName();
    
private:
    juce::OwnedArray<juce::Component> components;
    bool signalPlot = false;
    std::vector<double> signalToPlotIn;
    std::vector<double> signalToPlotOut;
    juce::Slider plotterX;
    juce::Slider plotterY;
    double plotterXScale = 1;
    double plotterYScale = 1;
    juce::Label inputText;
    juce::ComboBox inputSignalComboBox;
    juce::Label inputTimeText;
    juce::Label inputTime;
    juce::Label inputFrequency;
    juce::Label inputFrequencyText;
    juce::Label inputAmplitude;
    juce::Label inputAmplitudeText;
    juce::Label outputText;
    juce::Label outputFileName;
    
    juce::TextButton buttonToremove;
};

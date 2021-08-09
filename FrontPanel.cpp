/*
  ==============================================================================

    FrontPanel.cpp
    Created: 29 Apr 2021 4:52:46pm
    Author:  Stef

  ==============================================================================
*/

#include "FrontPanel.h"

FrontPanel::FrontPanel(){
    addChildComponent(plotterX);
    plotterX.setVisible(true);
    plotterX.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    plotterX.onValueChange = [this](){
        plotterXScale = plotterX.getValue();
        repaint();
    };
    plotterX.setAlwaysOnTop(true);
    plotterX.setRange(0.1,10,0.1);
    plotterX.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxLeft, true, 35, 20);
    addChildComponent(plotterY);
    plotterY.setVisible(true);
    plotterY.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    plotterY.onValueChange = [this](){
        plotterYScale = plotterY.getValue();
        repaint();
    };
    plotterY.setAlwaysOnTop(true);
    plotterY.setRange(0.1,100,0.1);
    plotterY.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxLeft, true, 35, 20);
    
    addAndMakeVisible(inputText);
    inputText.setText("Input Settings", juce::NotificationType::dontSendNotification);
    inputText.setEditable(false);
    addAndMakeVisible(inputSignalComboBox);
    inputSignalComboBox.addItem("File", 1);
    inputSignalComboBox.addItem("Sine",2);
    inputSignalComboBox.addItem("Square", 3);
    inputSignalComboBox.addItem("Impulse", 4);
    inputSignalComboBox.onChange = [this](){
        if(setSignalType != nullptr){
            setSignalType(inputSignalComboBox.getSelectedId());
        }
    };
    juce::Label inputTimeText;
    juce::Label inputTime;
    addAndMakeVisible(inputFrequency);
    inputFrequency.setEditable(true);
    inputFrequency.onTextChange = [this](){
        if(setSignalFreq != nullptr){
            setSignalFreq(inputFrequency.getText().getDoubleValue());
        }
    };
    juce::Label inputFrequencyText;
    
}
void FrontPanel::paint (juce::Graphics& g){
    g.setColour(juce::Colours::grey);
    g.fillAll(juce::Colours::grey);
    
    juce::Rectangle<float> rect(getWidth()-350, 0,350,getHeight());
    g.setColour(juce::Colours::black);
    g.fillRect(rect);
    
    float x = getWidth()-350;
    float y = getHeight()/2;
    juce::Path signalIn;
    juce::Path signalOut;

    signalIn.startNewSubPath(x, y);
    signalOut.startNewSubPath(x, y);

    auto numberOfPoints = signalToPlotIn.size();
    for (int i = 1; i < numberOfPoints; ++i)
    {
    // calculate or optain the coordinates x and y of the current point
       
        x += plotterXScale;
        y = (getHeight()/2)+plotterYScale*signalToPlotIn[i];
        signalIn.lineTo(x, y);
        
        y = (getHeight()/2)+plotterYScale*signalToPlotOut[i];
        signalOut.lineTo(x, y);
    }

    float lineThickness = 1.0f;
    g.setColour(juce::Colours::white);
    g.strokePath (signalIn, juce::PathStrokeType(lineThickness));
    g.setColour(juce::Colours::yellow);
    g.strokePath (signalOut, juce::PathStrokeType(lineThickness));
    
}
void FrontPanel::addNewComponent(juce::Component* c){
    int x = 0;
    ((juce::Slider*)(c))->setSliderStyle(juce::Slider::SliderStyle::Rotary);
    if(components.isEmpty()) x = 15;
    else x += components.getLast()->getX() + 115;
    ((juce::Slider*)(c))->setBounds(x,15,100,100);
    ((juce::Slider*)(c))->setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true,50,20);
    
    addAndMakeVisible(components.add(c));
}

void FrontPanel::addNewComponent(juce::Component* juceComp, std::function<void()> callback)
{
    int x = 0;
    ((juce::Slider*)(juceComp))->setSliderStyle(juce::Slider::SliderStyle::Rotary);
    if(components.isEmpty()) x = 15;
    else x += components.getLast()->getX() + 115;
    ((juce::Slider*)(juceComp))->setBounds(x,15,100,100);
    ((juce::Slider*)(juceComp))->setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true,50,20);
    ((juce::Slider*)(juceComp))->onValueChange = callback;

    addAndMakeVisible(components.add(juceComp));
}

void FrontPanel::resized(){
    plotterX.setBounds(getWidth()-345, getHeight()-30, 150, 30);
    plotterY.setBounds(getWidth()-345+155, getHeight()-30, 150, 30);
    inputText.setBounds(getWidth()-500, 0, 100, 15);
    inputSignalComboBox.setBounds(getWidth()-500, 20, 100, 20);
    inputFrequency.setBounds(getWidth()-500, 45, 100, 15);
}

void FrontPanel::removeComponent(juce::Component* c){
    removeChildComponent(c);
    components.removeObject(c);
    int x = 15;
    for(auto c: components){
        c->setBounds(x,15,100,100);
        x += 115;
    }
    //removeChildComponent(c);
}

void FrontPanel::plotSignal(bool shouldPlot){
    signalPlot = shouldPlot;
}

void FrontPanel::addSignalToPlot(std::vector<double> signalIn, std::vector<double> signalOut){
    signalToPlotIn = signalIn;
    signalToPlotOut = signalOut;
    repaint();
}
    

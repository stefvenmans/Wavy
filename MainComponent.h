#pragma once

#include <JuceHeader.h>
#include <functional>
#include <stdlib.h>
#include "../rt-wdf_lib/Libs/rt-wdf/rt-wdf.h"
#include "WDFEnvironment.h"
#include "CircuitComponent.h"
#include "Config.h"
#include "AdaptedLeafComponent.h"
#include "OnePortComponent.h"
#include "Resistor.h"
#include "Capacitor.h"
#include "VoltageSource.h"
#include "TwoPortComponent.h"
#include "Inverter.h"
#include "YParameterAmp.h"
#include "ThreePortComponent.h"
#include "Series.h"
#include "Parallel.h"
#include "SimpleRootComponent.h"
#include "IdealVoltageSource.h"
#include "ShortCircuit.h"
#include "RNodeRootComponent.h"
#include "NonLinearComponent.h"
#include "Diode.h"
#include "Transistor.h"
#include "RNodeNonLinRootComponent.h"

#include "FrontPanel.h"
#include "Schematic.h"
#include "PropertyPanel.h"
#include "ControlPanel.h"




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
    bool wantsToConnect_(CircuitComponent* c);
    
    static void printSomething(){
        std::cout << "hello world" << std::endl;
    }
    
    void mouseMagnify (const juce::MouseEvent &event, float scaleFactor) override;
    
    void openPropertyPanelForComponent(CircuitComponent* c);
    
    
    void readAudio();
    void processAudio();
    void writeAudio();
    
private:
    //==============================================================================
    // Your private member variables go here...
    
    WdfEnvironment wdfEnvironment;
    
    FrontPanel frontPanel;
    Schematic schematic;
    
    
    //DraggableComp draggableComp;
    juce::OwnedArray<juce::Component> components;
    juce::OwnedArray<AdaptedLeafComponent> leafComponents;
    juce::OwnedArray<NonLinearComponent> nonLinearComponents;
    
    juce::TextButton textButton;
    juce::Slider freqSlider;
    juce::ComboBox componentSelector;
    juce::TextButton showLibraryButton;
    juce::TextButton calculateMatButton;
    juce::TextButton calculateRaw;
    juce::TextButton runSimulationButton;
    bool runSimulation = false;
    
    juce::Viewport viewPort;
    //juce::BlockField blockField;
    
    
    PropertyPanel propertyPanel;
    juce::Viewport propertyPanelViewPort;
    bool propertyPanelShowHide = false;
    
    ControlPanel controlPanel;
    
    
    void setOutput(CircuitComponent* c);
    void setInput(CircuitComponent* c);
    
    int outputIndex = -1;
    int inputIndex = -1;
    
    float scale = 1.0f;
    paramData res1Val;
    
    juce::SidePanel sidePanel{"Library",200,true,nullptr,false};
    
    
    
    std::unique_ptr<SimpleRootComponent> simpleRoot;
    std::unique_ptr<RNodeRootComponent> rNode;
    std::unique_ptr<RNodeNonLinRootComponent> rNodeRootNonLin;
    
    
    juce::AudioFormatManager formatManager;
    juce::AudioSampleBuffer buffer_in;
    juce::AudioSampleBuffer buffer_out;
    juce::AudioFormatReader *reader;
    
    mat Smat;
    //Smat.set_size(6, 6);

    juce::dsp::Oscillator<float> osc {[](float x) {return std::sin(x);}};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};



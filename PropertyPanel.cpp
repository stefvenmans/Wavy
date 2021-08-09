/*
  ==============================================================================

    PropertyPanel.cpp
    Created: 29 Apr 2021 4:53:39pm
    Author:  Stef

  ==============================================================================
*/

#include "PropertyPanel.h"

PropertyPanel::PropertyPanel()
{
    addAndMakeVisible(componentType);
    componentType.setJustificationType(juce::Justification::centred);
    componentType.setBounds(0,0,250,25);
    
    addChildComponent(componentName);
    componentName.setEditable(true);
    componentName.setBounds(180,25,250-180,25);
    componentName.addListener(this);
    
    addChildComponent(componentNameText);
    componentNameText.setEditable(false);
    componentNameText.setBounds(0,25,180,25);
    componentNameText.setText("Name", juce::NotificationType::dontSendNotification);
    componentName.addListener(this);
    
    addChildComponent(componentValue1);
    componentValue1.setEditable(true);
    componentValue1.setBounds(180,25*2,250-180,25);
    componentValue1.addListener(this);
    
    addChildComponent(componentValue1Text);
    componentValue1Text.setEditable(false);
    componentValue1Text.setBounds(0,25*2,180,25);
    componentValue1Text.addListener(this);
    
    addChildComponent(componentValue2);
    componentValue2.setEditable(true);
    componentValue2.setBounds(180,25*3,250-180,25);
    componentValue2.addListener(this);
    
    addChildComponent(componentValue2Text);
    componentValue2Text.setEditable(false);
    componentValue2Text.setBounds(0,25*3,180,25);
    componentValue2Text.addListener(this);
    
    addChildComponent(setOutputText);
    setOutputText.setEditable(false);
    setOutputText.setBounds(0,25*4,180,25);
    setOutputText.setText("Set output", juce::NotificationType::dontSendNotification);
    setOutputText.addListener(this);
    
    addChildComponent(setInputText);
    setInputText.setEditable(false);
    setInputText.setBounds(0,25*4,180,25);
    setInputText.setText("Set input", juce::NotificationType::dontSendNotification);
    setInputText.addListener(this);
    
    addChildComponent(setControlText);
    setControlText.setEditable(false);
    setControlText.setBounds(0,25*5,180,25);
    setControlText.setText("Set control", juce::NotificationType::dontSendNotification);
    setControlText.addListener(this);
    
    
    addAndMakeVisible(setOutput);
    setOutput.setBounds(180,25*4,25,25);
    setOutput.addListener(this);
    
    addAndMakeVisible(setInput);
    setInput.setBounds(180,25*4,25,25);
    setInput.addListener(this);
    
    addAndMakeVisible(setControl);
    setControl.setBounds(180, 25*5, 25, 25);
    setControl.addListener(this);
    
    for(auto i=0; i<8; i++){
        addChildComponent(rnodeNullorStamp.add(new juce::Label()));
        if(i % 2 ==0){
            rnodeNullorStamp.getLast()->setBounds(i*25,25*7,25,25);
        }
        else{
            rnodeNullorStamp.getLast()->setBounds(i*25-10,25*7,25,25);
        }
        switch(i){
            case 0:
                rnodeNullorStamp.getLast()->setText("i:", juce::NotificationType::dontSendNotification);
                rnodeNullorStamp.getLast()->setEditable(false);
                break;
            case 2:
                rnodeNullorStamp.getLast()->setText("j:", juce::NotificationType::dontSendNotification);
                rnodeNullorStamp.getLast()->setEditable(false);
                break;
            case 4:
                rnodeNullorStamp.getLast()->setText("k:", juce::NotificationType::dontSendNotification);
                rnodeNullorStamp.getLast()->setEditable(false);
                break;
            case 6:
                rnodeNullorStamp.getLast()->setText("l:", juce::NotificationType::dontSendNotification);
                rnodeNullorStamp.getLast()->setEditable(false);
                break;
            default:
                rnodeNullorStamp.getLast()->setEditable(true);
                rnodeNullorStamp.getLast()->addListener(this);
                break;
        }
        
    }
    
    addChildComponent(rnodeNullorComboBox);
    rnodeNullorComboBox.setBounds(150,25*6+2.5,100,20);
    rnodeNullorComboBox.setVisible(true);
    rnodeNullorComboBox.onChange = [this](){
        ((RNodeRootComponent*)componentLastSelected)->setLastSelectedNullor(rnodeNullorComboBox.getSelectedId()-1);
        setPropertiesForComponent(componentLastSelected);
    };
    
    addChildComponent(addNullorButton);
    addNullorButton.setBounds(2.5,25*6+2.5,100,20);
    addNullorButton.addListener(this);
    addNullorButton.setButtonText("Create Nullor");
    
    for(auto i=0; i<30; i++){
        textBoxes.add(new juce::Rectangle<float>(0,0+i*25,250,25));
    }
    
    
}

void PropertyPanel::paint (juce::Graphics& g) {
    g.setColour(juce::Colours::darkgrey);
    g.fillAll();
    
    
    bool togleColour = false;
    for(auto r : textBoxes){
        if(togleColour){
            g.setColour(juce::Colour(38 + 15,47 + 15,53 + 15));
        }
        else{
            g.setColour(juce::Colour(38,47,53));
        }
        togleColour = !togleColour;
        g.fillRect(*r);
    }
}

void PropertyPanel::setPropertiesForComponent(CircuitComponent* c){
    componentLastSelected = c;
    switch(c->getComponentType()){
        case L_RES:
            componentType.setText("Resistor",juce::NotificationType::dontSendNotification);
            componentNameText.setVisible(true);
            componentName.setText(c->getName(),juce::NotificationType::dontSendNotification);
            componentName.setVisible(true);
            componentValue1Text.setText("Resistance (ohm)", juce::NotificationType::dontSendNotification);
            componentValue1Text.setVisible(true);
            componentValue1.setText(juce::String(((Resistor*)c)->getR()),juce::NotificationType::dontSendNotification);
            componentValue1.setVisible(true);
            componentValue2Text.setVisible(false);
            componentValue2.setVisible(false);
            setOutputText.setVisible(true);
            setOutput.setVisible(true);
            setInput.setVisible(false);
            setInputText.setVisible(false);
            setControlText.setVisible(true);
            break;
        case L_CAP:
            componentType.setText("Capacitor",juce::NotificationType::dontSendNotification);
            componentNameText.setVisible(true);
            componentName.setText(c->getName(),juce::NotificationType::dontSendNotification);
            componentName.setVisible(true);
            componentValue1Text.setText("Capacitance (F)", juce::NotificationType::dontSendNotification);
            componentValue1Text.setVisible(true);
            componentValue1.setText(juce::String(((Capacitor*)c)->getC()),juce::NotificationType::dontSendNotification);
            componentValue1.setVisible(true);
            componentValue2Text.setVisible(false);
            componentValue2.setVisible(false);
            setOutputText.setVisible(true);
            setOutput.setVisible(true);
            setInput.setVisible(false);
            setInputText.setVisible(false);
            setControlText.setVisible(true);
            break;
        case L_VOL:
            componentType.setText("Resistive Voltage Source",juce::NotificationType::dontSendNotification);
            componentNameText.setVisible(true);
            componentName.setText(c->getName(),juce::NotificationType::dontSendNotification);
            componentName.setVisible(true);
            componentValue1Text.setText("Voltage (V)", juce::NotificationType::dontSendNotification);
            componentValue1Text.setVisible(true);
            componentValue1.setText(juce::String(((VoltageSource*)c)->getVs()),juce::NotificationType::dontSendNotification);
            componentValue1.setVisible(true);
            componentValue2Text.setText("Resistance (ohm)", juce::NotificationType::dontSendNotification);
            componentValue2Text.setVisible(true);
            componentValue2.setText(juce::String(((VoltageSource*)c)->getRs()),juce::NotificationType::dontSendNotification);
            componentValue2.setVisible(true);
            setOutputText.setVisible(false);
            setOutput.setVisible(false);
            setInputText.setVisible(true);
            setInput.setVisible(true);
            setControlText.setVisible(true);
            break;
        case A_INV:
            componentType.setText("Inverter",juce::NotificationType::dontSendNotification);
            componentNameText.setVisible(true);
            componentName.setText(c->getName(),juce::NotificationType::dontSendNotification);
            componentName.setVisible(true);
            componentValue1.setVisible(false);
            componentValue1Text.setVisible(false);
            componentValue2.setVisible(false);
            componentValue2Text.setVisible(false);
            setOutputText.setVisible(false);
            setOutput.setVisible(false);
            setInputText.setVisible(false);
            setInput.setVisible(false);
            setControlText.setVisible(false);
            break;
        case A_SER:
            componentType.setText("Serries",juce::NotificationType::dontSendNotification);
            componentNameText.setVisible(true);
            componentName.setText(c->getName(),juce::NotificationType::dontSendNotification);
            componentName.setVisible(true);
            componentValue1.setVisible(false);
            componentValue1Text.setVisible(false);
            componentValue2.setVisible(false);
            componentValue2Text.setVisible(false);
            setOutputText.setVisible(false);
            setOutput.setVisible(false);
            setInputText.setVisible(false);
            setInput.setVisible(false);
            setControlText.setVisible(false);
            break;
        case A_PAR:
            componentType.setText("Parallel",juce::NotificationType::dontSendNotification);
            componentNameText.setVisible(true);
            componentName.setText(c->getName(),juce::NotificationType::dontSendNotification);
            componentName.setVisible(true);
            componentValue1.setVisible(false);
            componentValue1Text.setVisible(false);
            componentValue2.setVisible(false);
            componentValue2Text.setVisible(false);
            setOutputText.setVisible(false);
            setOutput.setVisible(false);
            setInputText.setVisible(false);
            setInput.setVisible(false);
            setControlText.setVisible(false);
            break;
            
        case SR_VOL:
            componentType.setText("Ideal Voltage Source",juce::NotificationType::dontSendNotification);
            componentNameText.setVisible(true);
            componentName.setText(c->getName(),juce::NotificationType::dontSendNotification);
            componentName.setVisible(true);
            componentValue1Text.setText("Voltage (V)", juce::NotificationType::dontSendNotification);
            componentValue1Text.setVisible(true);
            componentValue1.setText(juce::String(((IdealVoltageSource*)c)->getVs()),juce::NotificationType::dontSendNotification);
            setOutputText.setVisible(false);
            setOutput.setVisible(false);
            setInputText.setVisible(true);
            setInput.setVisible(true);
            componentValue1.setVisible(true);
            break;
        case R_RNODE:
            componentType.setText("R Node",juce::NotificationType::dontSendNotification);
            componentNameText.setVisible(true);
            componentName.setText(c->getName(),juce::NotificationType::dontSendNotification);
            componentName.setVisible(true);
            setControlText.setVisible(false);
            addNullorButton.setVisible(true);
            for(auto l: rnodeNullorStamp){
                l->setVisible(true);
            }
            
            auto nullorIndexes = ((RNodeRootComponent*)componentLastSelected)->getNullorStampIndexes();
            if(nullorIndexes.size()!=0){
                for(auto i=0; i<4; i++){
                    rnodeNullorStamp[(1)+2*i]->setText(juce::String(nullorIndexes[i+(rnodeNullorComboBox.getSelectedId()-1)*4]), juce::NotificationType::dontSendNotification);
                }
            }
            
            for(auto i: ((RNodeRootComponent*)componentLastSelected)->getNullorStampIndexes()){
                std::cout << "nullor from prop: " << i << std::endl;
            }
            
            break;
    }
    
}

void PropertyPanel::labelTextChanged (juce::Label *labelThatHasChanged) {
    if(labelThatHasChanged == &componentName){
        componentLastSelected->setName(componentName.getText());
    }
    switch(componentLastSelected->getComponentType()){
        case L_RES:
            ((Resistor*)componentLastSelected)->setR(componentValue1.getText().getDoubleValue());
            break;
        case L_CAP:
            ((Capacitor*)componentLastSelected)->setC(componentValue1.getText().getDoubleValue());
            break;
        case L_VOL:
            if (labelThatHasChanged == &componentValue1){
                ((VoltageSource*)componentLastSelected)->setVs(componentValue1.getText().getDoubleValue());
            }
            else if (labelThatHasChanged == &componentValue2){
                ((VoltageSource*)componentLastSelected)->setRs(componentValue2.getText().getDoubleValue());
            }
            break;
        case A_INV:
            
            break;
        case A_SER:
            
            break;
        case A_PAR:
            
            break;
        case SR_VOL:
            ((IdealVoltageSource*)componentLastSelected)->setVs(componentValue1.getText().getDoubleValue());
            break;
            
    }
}

void PropertyPanel::setOutputCallbackFunction(std::function<void(CircuitComponent* c)> callback){
    setOutputOfCircuit = callback;
}

void PropertyPanel::setInputCallbackFunction(std::function<void(CircuitComponent*cc)> callback){
    setInputOfCircuit = callback;
}

void PropertyPanel::buttonClicked (juce::Button * b) {
    //std::cout << "togle state changed" << std::endl;
    if(b == &setOutput){
        if(setOutput.getToggleState()==true){
            std::cout << "output will be set" << std::endl;
            setOutputOfCircuit(componentLastSelected);
        }
    }
    else if(b == &setInput){
        if(setInput.getToggleState()==true){
            std::cout << "input will be set" << std::endl;
            setInputOfCircuit(componentLastSelected);
        }
    }
    else if(b == &setControl){
        if(setControl.getToggleState() == true){
            juce::Slider* s = new juce::Slider();
            auto c = componentLastSelected;
            switch(componentLastSelected->getComponentType()){
                case L_RES:
                    //s = new juce::Slider();
                    c = componentLastSelected;
                    s->onValueChange = [this,c, s](){
                        ((Resistor*)(c))->setR(s->getValue());
                        if(componentLastSelected == c){
                            setPropertiesForComponent(c);
                        }
                    };
                    frontPanel->addNewComponent(s);
                    componentLastSelected->setControl(s);
                    break;
                case L_CAP:
                    //s = new juce::Slider();
                    c = componentLastSelected;
                    s->onValueChange = [this,c, s](){
                        ((Capacitor*)(c))->setC(s->getValue());
                        if(componentLastSelected == c){
                            setPropertiesForComponent(c);
                        }
                    };
                    frontPanel->addNewComponent(s);
                    componentLastSelected->setControl(s);
                    break;
                case L_VOL:
                    //s = new juce::Slider();
                    c = componentLastSelected;
                    s->onValueChange = [this,c, s](){
                        ((VoltageSource*)(c))->setVs(s->getValue());
                        if(componentLastSelected == c){
                            setPropertiesForComponent(c);
                        }
                    };
                    frontPanel->addNewComponent(s);
                    componentLastSelected->setControl(s);
                    break;
                case SR_VOL:
                    //s = new juce::Slider();
                    c = componentLastSelected;
                    s->onValueChange = [this,c, s](){
                        ((IdealVoltageSource*)(c))->setVs(s->getValue());
                        if(componentLastSelected == c){
                            setPropertiesForComponent(c);
                        }
                    };
                    frontPanel->addNewComponent(s);
                    componentLastSelected->setControl(s);
                    break;
            }
        }
        else{
            frontPanel->removeComponent(componentLastSelected->getControl());
            componentLastSelected->setControl(nullptr);
        }
    }
    
    else if(b == &addNullorButton){
        ((RNodeRootComponent*)componentLastSelected)->createNullorStamps();
        rnodeNullorComboBox.addItem("Nullor"+juce::String(((RNodeRootComponent*)componentLastSelected)->getNullorStampIndexes().size()/4), rnodeNullorComboBox.getNumItems()+1);
        rnodeNullorComboBox.setSelectedId(rnodeNullorComboBox.getNumItems());
        setPropertiesForComponent(componentLastSelected);
    }
}
    
void PropertyPanel::setFrontPanel(FrontPanel* fPanel){
    frontPanel = fPanel;
}

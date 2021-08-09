#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    
    setWantsKeyboardFocus(true);
    addKeyListener(this);
    
    formatManager.registerBasicFormats();
    auto file = appDataPath.getChildFile ("input.wav");
    reader = formatManager.createReaderFor(file);
    
    
    //GUI
    
    addAndMakeVisible(controlPanel);
    
    //TODO: set size in settings window
    addAndMakeVisible(schematic);
    schematic.centreWithSize(820*5, 580*5);
    
    viewPort.setViewedComponent(&schematic, false);
    addAndMakeVisible(&viewPort);
    viewPort.setBounds(0, 0, 820, 420);
    viewPort.setScrollBarsShown(true,true);
    
    schematic.hidePropertyPanelCallbck = [this]() {
        propertyPanelShowHide = false;
        schematic.repaint();
        resized();
    };
    
    addAndMakeVisible(propertyPanel);
    propertyPanelViewPort.setViewedComponent(&propertyPanel, false);
    addChildComponent(propertyPanelViewPort);
    propertyPanelViewPort.setScrollBarsShown(true, false);
    propertyPanel.setBounds(0, 0, 250, 1000);
    propertyPanel.setOutputCallbackFunction(std::bind(&MainComponent::setOutput,this,std::placeholders::_1));
    propertyPanel.setInputCallbackFunction(std::bind(&MainComponent::setInput,this,std::placeholders::_1));
    propertyPanel.setFrontPanel(&frontPanel);
    
    addAndMakeVisible(frontPanel);
    frontPanel.setBounds(0,420,820,580-420);
    
    //Create input signals;
    //sine
    std::vector<double> sine;
    for(int i=0; i<inputSignalDuration*44100; i++){
        sine.push_back(std::sin(inputSignalFrequency*i*(juce::MathConstants<double>::pi*2.0)/44100.0));
    }
    inputSignals.push_back(sine);
    
    std::vector<double> square;
    for(int i=0; i<inputSignalDuration*44100; i++){
        if(std::sin(inputSignalFrequency*i*(juce::MathConstants<double>::pi*2.0)/44100.0)>0){
            square.push_back(-1);
        }
        else{
            square.push_back(1);
        }
    }
    inputSignals.push_back(square);
    
    std::vector<double> impulse;
    for(int i=0; i<inputSignalDuration*44100; i++){
        if(i==0){
            impulse.push_back(1);
        }
        else{
            impulse.push_back(0);
        }
    }
    inputSignals.push_back(impulse);
    
    frontPanel.setSignalType = [this](int i){
        inputSignalType = i-2;
    };
    
    frontPanel.setSignalFreq = [this](double f){
        
    };
    
    
    addAndMakeVisible(calculateMatButton);
    calculateMatButton.setButtonText("Mat");
    calculateMatButton.onClick = [this](){
        
    };
    
    addAndMakeVisible(textButton);
    textButton.onClick = [this](){
        
        if(simpleRoot != nullptr){
            wdfEnvironment.setSubtreeEntryNodes(simpleRoot->getChildComponent()->createWDFComponent());
            wdfEnvironment.setRoot(simpleRoot->createWDFComponent());
        }
        else if(rNode != nullptr){

            wdfEnvironment.setSMat(rNode->calculateScatteringMatrix());
            
            wdfEnvironment.setSubtreeEntryNodes(rNode->getChildsWDFTreeNodes());
        }

        wdfEnvironment.initTree();
        wdfEnvironment.adaptTree();

        std::cout << "Tree will be calculated for input:  " << inputIndex << "  and output :  " << outputIndex << std::endl;
        
        readAudio();
        processAudio();
        writeAudio();
        
//        for(auto i=0; i<500; i++){
//            wdfEnvironment.cycleWave();
//            if(outputIndex != -1 && outputIndex < leafComponents.size()){
//                std::cout << -leafComponents[outputIndex]->getWDFComponent()->upPort->getPortVoltage() << std::endl;
//            }
//            else std::cout << "No output set!" << std::endl;
//        }
    };
    
    addAndMakeVisible(showLibraryButton);
    //showLibraryButton.setBounds(600, 400, 40, 40);
    showLibraryButton.onClick = [this] () {
//        std::cout << sidePanel.Component::isShowing() << std::endl;
//        if(sidePanel.isPanelShowing() == false){
//            sidePanel.showOrHide(true);
//            setSize(1000,getHeight());
//            viewPort.setBounds(sidePanel.getWidth(), 0, getWidth()-sidePanel.getWidth(), getHeight()-160);
//        frontPanel.setBounds(sidePanel.getWidth(),getHeight()-160,getWidth()-sidePanel.getWidth(),580-420);
//        }
//        else{
//            viewPort.setBounds(0, 0, getWidth(), getHeight()-160);
//            frontPanel.setBounds(0,getHeight()-160,getWidth(),580-420);
//            sidePanel.showOrHide(false);
//        }
        propertyPanelShowHide = false;
        
        schematic.repaint();
        
        resized();
    };
    
    
//    addAndMakeVisible(sidePanel);
//    sidePanel.setBounds(300,300,200,300);
    
    componentSelector.addItem("Resistor", 1);
    componentSelector.addItem("Capacitor",2);
    componentSelector.addItem("Inverter", 3);
    componentSelector.addItem("Y-Parameter Amplifier",4);
    componentSelector.addItem("Series", 5);
    componentSelector.addItem("Parallel",6);
    componentSelector.addItem("IdealVolSource",7);
    componentSelector.addItem("R Node",8);
    componentSelector.addItem("Resistive Voltage Source",9);
    componentSelector.addItem("Short Circuit",10);
    componentSelector.addItem("-",11);
    componentSelector.addItem("Old R Node", 12);
    componentSelector.addItem("Diode", 13);
    componentSelector.addItem("Transistor",14);
    componentSelector.addItem("R Node Non Lin", 15);
    addAndMakeVisible(componentSelector);
    
    
    
    componentSelector.onChange = [this](){
        std::function<void(CircuitComponent*c)> componentIsSelectedCallBack = [this] (CircuitComponent*c)
        {
            for(auto c : selectedComponents){
                c->unSelectComponent();
            }
            selectedComponents.clear();
            selectedComponents.push_back(c);
        };
        switch(componentSelector.getSelectedId()){
            case 1:
                schematic.addAndMakeVisible(leafComponents.add(new Resistor()));
                leafComponents.getLast()->setName("R1");
                leafComponents.getLast()->setBounds(20,20,100,100);
                leafComponents.getLast()->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                leafComponents.getLast()->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                leafComponents.getLast()->componentIsSelectedCallBack = componentIsSelectedCallBack;
                break;
                
            case 2:
                schematic.addAndMakeVisible(leafComponents.add(new Capacitor()));
                leafComponents.getLast()->setName("C1");
                leafComponents.getLast()->setBounds(20,20,100,100);
                leafComponents.getLast()->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                leafComponents.getLast()->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                leafComponents.getLast()->componentIsSelectedCallBack = componentIsSelectedCallBack;
                break;
                
            case 3:
                schematic.addAndMakeVisible(leafComponents.add(new Inverter()));
                leafComponents.getLast()->setName("I1");
                leafComponents.getLast()->setBounds(20,20,100,100);
                leafComponents.getLast()->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                leafComponents.getLast()->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                leafComponents.getLast()->componentIsSelectedCallBack = componentIsSelectedCallBack;
                break;
                
            case 4:
                schematic.addAndMakeVisible(leafComponents.add(new YParameterAmp()));
                leafComponents.getLast()->setName("y-1");
                leafComponents.getLast()->setBounds(20,20,200,100);
                leafComponents.getLast()->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                leafComponents.getLast()->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                leafComponents.getLast()->componentIsSelectedCallBack = componentIsSelectedCallBack;
                break;
                
            case 5:
                schematic.addAndMakeVisible(leafComponents.add(new Series()));
                leafComponents.getLast()->setName("S1");
                leafComponents.getLast()->setBounds(20,20,100,100);
                leafComponents.getLast()->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                leafComponents.getLast()->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                leafComponents.getLast()->componentIsSelectedCallBack = componentIsSelectedCallBack;
                break;
                
            case 6:
                schematic.addAndMakeVisible(leafComponents.add(new Parallel()));
                leafComponents.getLast()->setName("P1");
                leafComponents.getLast()->setBounds(20,20,100,100);
                leafComponents.getLast()->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                leafComponents.getLast()->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                leafComponents.getLast()->componentIsSelectedCallBack = componentIsSelectedCallBack;
                break;
            case 7:
                simpleRoot = std::make_unique<IdealVoltageSource>();
                simpleRoot->setName("V1");
                schematic.addAndMakeVisible(simpleRoot.get());
                simpleRoot->setBounds(20,20,100,100);
                simpleRoot->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                simpleRoot->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                simpleRoot->componentIsSelectedCallBack = componentIsSelectedCallBack;
                break;
            case 8:
                rNode = std::make_unique<RNodeRootComponent>();
                schematic.addAndMakeVisible(rNode.get());
                rNode->setCentrePosition(200, 200);
                rNode->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                rNode->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                rNode->componentIsSelectedCallBack = componentIsSelectedCallBack;
                break;
            case 9:
                schematic.addAndMakeVisible(leafComponents.add(new VoltageSource()));
                leafComponents.getLast()->setName("V1");
                leafComponents.getLast()->setBounds(20,20,100,100);
                leafComponents.getLast()->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                leafComponents.getLast()->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                leafComponents.getLast()->componentIsSelectedCallBack = componentIsSelectedCallBack;
                break;
            case 10:
                simpleRoot = std::make_unique<ShortCircuit>();
                simpleRoot->setName("Shrt1");
                schematic.addAndMakeVisible(simpleRoot.get());
                simpleRoot->setBounds(20,20,100,100);
                simpleRoot->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                simpleRoot->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                simpleRoot->componentIsSelectedCallBack = componentIsSelectedCallBack;
                break;
            case 11:
                break;
            case 13:
                schematic.addAndMakeVisible(nonLinearComponents.add(new Diode()));
                nonLinearComponents.getLast()->setName("D1");
                nonLinearComponents.getLast()->setBounds(20,20,100,100);
                nonLinearComponents.getLast()->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                nonLinearComponents.getLast()->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                break;
            case 14:
                schematic.addAndMakeVisible(nonLinearComponents.add(new Transistor()));
                nonLinearComponents.getLast()->setName("Q1");
                nonLinearComponents.getLast()->setBounds(20,20,200,100);
                nonLinearComponents.getLast()->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                nonLinearComponents.getLast()->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                break;
            case 15:
                rNodeRootNonLin = std::make_unique<RNodeNonLinRootComponent>();
                rNodeRootNonLin->setName("Rn");
                schematic.addAndMakeVisible(rNodeRootNonLin.get());
                rNodeRootNonLin->setCentrePosition(200, 200);
                rNodeRootNonLin->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                rNodeRootNonLin->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                break;
        }
    };
    
    
    //WDF param
    res1Val.name = "Resistor1";
    res1Val.ID = 0;
    res1Val.type = doubleParam;
    res1Val.value = 1000;
    res1Val.units = "ohms";
    res1Val.lowLim = 1e-3;
    res1Val.highLim = 1e4;
    
    freqSlider.setRange(100, 2000);
    freqSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    freqSlider.setValue(440);
    addAndMakeVisible(freqSlider);
    freqSlider.onDragEnd = [this](){
        osc.setFrequency(freqSlider.getValue());
    };
    
    wdfEnvironment.addParam(res1Val);
    
    
    // Library
    addAndMakeVisible(&sidePanel);
    sidePanel.setShadowWidth(0);
    setSize (820, 580);
    sidePanel.showOrHide(false);
    
    addAndMakeVisible(calculateRaw);
    calculateRaw.setButtonText("Simulate");
    calculateRaw.onClick = [this] () {
        if(simpleRoot != nullptr || rNode != nullptr || rNodeRootNonLin!=nullptr){
            if(simpleRoot != nullptr){
                    wdfEnvironment.setSubtreeEntryNodes(simpleRoot->getChildComponent()->createWDFComponent());
                    wdfEnvironment.setRoot(simpleRoot->createWDFComponent());
                    wdfEnvironment.initTree();
                    wdfEnvironment.adaptTree();
                }
            else if(rNode != nullptr){
                //auto sMatrix = rNode->calculateScatteringMatrix();
                wdfEnvironment.setSubtreeEntryNodes(rNode->getChildsWDFTreeNodes());
                
                //wdfEnvironment.adaptTree();
                wdfEnvironment.getRNodeMatLambda = [this](matData* rootMatrixData, double *Rp){
                    return rNode->calculateScatteringMatrix(rootMatrixData, Rp);
                };
                
                wdfEnvironment.initTree();
                wdfEnvironment.adaptTree();
                
                }
            
            else if(rNodeRootNonLin!=nullptr){
                wdfEnvironment.setSubtreeEntryNodes(rNodeRootNonLin->getChildsWDFTreeNodes(), rNodeRootNonLin->getNLlist());
                
                //wdfEnvironment.adaptTree();
                wdfEnvironment.getRNodeMatLambda = [this](matData* rootMatrixData, double *Rp){
                    return rNodeRootNonLin->calculateScatteringMatrix(rootMatrixData, Rp);
                };
                
                wdfEnvironment.initTree();
                wdfEnvironment.adaptTree();
            }
            
//            if(inputIndex != -1){
//                readAudio();
//                processAudio();
//                writeAudio();
//            }
            //else {
                
                std::vector<double> signalToPlotIn;
                std::vector<double> signalToPlotOut;
                frontPanel.plotSignal(true);
                
                for(auto i=0; i<inputSignalDuration*44100; i++){
                if(inputIndex != -1 && inputIndex < leafComponents.size()){
//                    if(i==0){
//                        signalToPlotIn.push_back(1);
//                        ((wdfTerminatedResVSource*)(leafComponents[inputIndex]->getWDFComponent()))->Vs = 1;//bufferIn[i];
//                    }
//                    else{
//                        signalToPlotIn.push_back(0);
//                        ((wdfTerminatedResVSource*)(leafComponents[inputIndex]->getWDFComponent()))->Vs = 0;//bufferIn[i];
                    //}
//                    signalToPlotIn.push_back(std::sin(i*(juce::MathConstants<double>::pi*2.0)/200.0));
//                    ((wdfTerminatedResVSource*)(leafComponents[inputIndex]->getWDFComponent()))->Vs = std::sin(i*(juce::MathConstants<double>::pi*2.0)/200.0);//bufferIn[i];
                    
                    ((wdfTerminatedResVSource*)(leafComponents[inputIndex]->getWDFComponent()))->Vs = inputSignals[inputSignalType][i];//bufferIn[i];
                }
                wdfEnvironment.cycleWave();
                if(outputIndex != -1 && outputIndex < leafComponents.size()){
                    //std::cout << -leafComponents[outputIndex]->getWDFComponent()->upPort->getPortVoltage() << std::endl;
                    
                        signalToPlotIn.push_back(inputSignals[inputSignalType][i]);
                        signalToPlotOut.push_back(leafComponents[outputIndex]->getWDFComponent()->upPort->getPortVoltage());
                    
                }
                else std::cout << "No output set!" << std::endl;
                }
                frontPanel.addSignalToPlot(signalToPlotIn,signalToPlotOut);
                
            //}
            
            if(inputIndex != -1){
                //wdfEnvironment.initTree();
                wdfEnvironment.adaptTree();
                readAudio();
                processAudio();
                writeAudio();
            }
            
        }
    };
    
    addAndMakeVisible(runSimulationButton);
    runSimulationButton.onClick = [this](){
        runSimulation = !runSimulation;
        (runSimulation) ? (std::cout << "Running") : (std::cout << "Not running") ;
        std::cout << std::endl;
    };
    runSimulationButton.setButtonText("Run");
    
    //Menubar
    menuBar.reset(new juce::MenuBarComponent(this));
    addAndMakeVisible(menuBar.get());
    juce::MenuBarModel::setMacMainMenu(this);
    
    setApplicationCommandManagerToWatch (&commandManager);
    commandManager.registerAllCommandsForTarget (this);
    

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        
        setAudioChannels (2, 2);
    }
    
}

void MainComponent::openPropertyPanelForComponent(CircuitComponent* c){
    propertyPanelShowHide = true;
    resized();
    propertyPanel.setPropertiesForComponent(c);
}

bool MainComponent::wantsToConnect_(CircuitComponent* c)
{
    auto cX = c->getX();
    auto cY = c->getY();
    auto cW = c->getWidth();
    auto cH = c->getHeight();
    
    for(auto i: leafComponents){
        //if(i->getX() + c->getRotationX()*100 == c->getX() && i->getY() + c->getRotationY()*100 == c->getY()){
        auto iX = i->getX();
        auto iY = i->getY();
        auto iW = i->getWidth();
        auto iH = i->getHeight();
        
        for(int j=0; j<c->getCollums(); j++){
            
            for(int k=0; k<c->getRows(); k++){
                if(((iX+iW)==cX && iY==(cY+componentHeight*k)) || ((iY+iH)==cY && iX==(cX+componentWidth*j)) || ((cX+cW)==iX && (cY+componentHeight*k)==iY) || ((cY+cH)==iY && (cX+componentWidth*j)==iX)){
                            std::cout << "circuit could connect"<< std::endl;
                            c->connect(i);
                            i->connect(c);
                
                            c->repaint();
                            i->repaint();
                        }
                }
            
        }
        
        
        
//        if(((iX+iW)==cX && iY==cY) || ((iY+iH)==cY && iX==cX) || ((cX+cW)==iX && cY==iY) || ((cY+cH)==iY && cX==iX)){
//            std::cout << "circuit could connect"<< std::endl;
//            c->connect(i);
//            i->connect(c);
//
//            c->repaint();
//            i->repaint();
//            //return true;
//        }
    }
    
    if(simpleRoot != nullptr && c!=rNode.get()){
        auto iX = simpleRoot.get()->getX();
        auto iY = simpleRoot.get()->getY();
        auto iW = simpleRoot.get()->getWidth();
        auto iH = simpleRoot.get()->getHeight();
        
        if(((iX+iW)==cX && iY==cY) || ((iY+iH)==cY && iX==cX) || ((cX+cW)==iX && cY==iY) || ((cY+cH)==iY && cX==iX)){
            std::cout << "a root want could connect to this component" << std::endl;
            c->connect(simpleRoot.get());
            simpleRoot->connect(c);
            
            c->repaint();
            simpleRoot->repaint();
        }
    }
    
    //if(rNode != nullptr && c!=simpleRoot.get() && c!= rNode.get()){
    if(rNode != nullptr && c!=simpleRoot.get()){
        
        
        for(auto i=0; i<rNode->getCollums(); i++){
            for(auto j=0; j<rNode->getRows(); j++){
                auto iX = rNode.get()->getX() + (rNode.get()->getWidth()/rNode->getCollums())*(i);
                auto iY = rNode.get()->getY() + (rNode.get()->getHeight()/rNode->getRows())*(j);
                auto iW = rNode.get()->getWidth();
                auto iH = rNode.get()->getHeight();
                
                if(((iX+iW)==cX && iY==cY) || ((iY+iH)==cY && iX==cX) || ((cX+cW)==iX && cY==iY) || ((cY+cH)==iY && cX==iX)){
                    std::cout << "a r node want could connect to this component" << std::endl;
                    c->connect(rNode.get());
                    rNode->connect(c);
                    
                    c->repaint();
                    rNode->repaint();
                }
            }
        }
    }
    
    if(rNodeRootNonLin != nullptr && c!=simpleRoot.get() && c!=rNode.get()){
        
        for(auto i=0; i<rNodeRootNonLin->getCollums(); i++){
            for(auto j=0; j<rNodeRootNonLin->getRows(); j++){
                auto iX = rNodeRootNonLin.get()->getX() + (rNodeRootNonLin.get()->getWidth()/rNodeRootNonLin->getCollums())*(i);
                auto iY = rNodeRootNonLin.get()->getY() + (rNodeRootNonLin.get()->getHeight()/rNodeRootNonLin->getRows())*(j);
                auto iW = rNodeRootNonLin.get()->getWidth();
                auto iH = rNodeRootNonLin.get()->getHeight();
                
                if(((iX+iW)==cX && iY==cY) || ((iY+iH)==cY && iX==cX) || ((cX+cW)==iX && cY==iY) || ((cY+cH)==iY && cX==iX)){
                    std::cout << "a r node root want could connect to this component" << std::endl;
                    
                    if(rNodeRootNonLin->connect(c) == 1){
                        c->connect(rNodeRootNonLin.get());
                        c->repaint();
                        rNodeRootNonLin->repaint();
                        std::cout << rNodeRootNonLin->getName() << " connected to " << c->getName() << std::endl;
                    } 
                }
            }
        }
    }
    
}

void MainComponent::setOutput(CircuitComponent* c){
    outputIndex = leafComponents.indexOf((AdaptedLeafComponent*)c);
}

void MainComponent::setInput(CircuitComponent* c){
    inputIndex = leafComponents.indexOf((AdaptedLeafComponent*)c);
}


MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
    delete reader;
    juce::MenuBarModel::setMacMainMenu (nullptr);
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
    
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlockExpected;
    spec.sampleRate = sampleRate;
    spec.numChannels = 2;
    
    osc.prepare(spec);
    
    std::cout << "samples per block" << samplesPerBlockExpected << std::endl;
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();
    //for(auto j=0; j<bufferToFill.buffer->getNumChannels() ; j++){
    for(auto j=0; j<1 ; j++){
        auto bufferIn = bufferToFill.buffer->getReadPointer(j);
        auto bufferOut = bufferToFill.buffer->getWritePointer(j);
        if(runSimulation){
            
            for(auto i=0; i<bufferToFill.numSamples ;i++){
                
                    //output[i] = wdf->getOutputValue();
                    
                    if(inputIndex != -1 && inputIndex < leafComponents.size()){
                        ((wdfTerminatedResVSource*)(leafComponents[inputIndex]->getWDFComponent()))->Vs = osc.processSample( 0);//bufferIn[i];
                    }
                    wdfEnvironment.cycleWave();
                    
                    if(outputIndex != -1 && outputIndex < leafComponents.size()){
                        //std::cout << -leafComponents[outputIndex]->getWDFComponent()->upPort->getPortVoltage() << std::endl;
                        //std::cout << -leafComponents[outputIndex]->getWDFComponent()->upPort->getPortVoltage() << std::endl;
                        bufferOut[i] = -leafComponents[outputIndex]->getWDFComponent()->upPort->getPortVoltage();
            //        std::cout << wdf->getOutputValue() << std::endl;
            //        std::cout << input[i] << std::endl;
                    }
                }
        }
        else {
            
            for(auto i=0; i<bufferToFill.numSamples ;i++){
                //bufferOut[i] = 0.1*osc.processSample( 0);
            }
        }
    }
    
    //bufferToFill.clearActiveBufferRegion();
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.fillAll(juce::Colours::white);
    
    

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.

    
    
    int numX = 5;
    int numY = 5;
    
    auto stepX = getWidth()/numX;
    auto stepY = getHeight()/numY;
    
    //textButton.setBounds(getWidth()-350,40,50,50);
    //showLibraryButton.setBounds(getWidth()-350,40*2+80,50,50);
    freqSlider.setBounds(160,getHeight()-30,150,30);
    componentSelector.setBounds(20,20,100,30);
    //calculateMatButton.setBounds(getWidth()-350,40*2+80+60,50,50);
    calculateRaw.setBounds(50, getHeight()-30, 100, 30);
    runSimulationButton.setBounds(getWidth()-350, getHeight()-30, 50, 30);
    
//    for (auto x = 0; x < numX; ++x)
//        {
//            for (auto y = 0; y < numY; ++y)
//            {
//                // creates the rectangle     (x,         y,         width, height)
//                juce::Rectangle<int> elementBounds (x * stepX, y * stepY, stepX, stepY);
//
//                // set the size and position of the Toggle light to this rectangle.
//                components[x + numX * y].setBounds (elementBounds);
//            }
//        }
    
    
    controlPanel.setBounds(0, getHeight()-30, getWidth(), 30);
    frontPanel.setBounds(0,getHeight()-160,getWidth(),580-420 - 30);
    
    if(propertyPanelShowHide){
        viewPort.setBounds(0, 0, getWidth()-250, getHeight()-160);
        propertyPanelViewPort.setBounds(getWidth()-250,0,250,getHeight()-160);
        propertyPanelViewPort.setVisible(true);
    }
    else{
        viewPort.setBounds(0, 0, getWidth(), getHeight()-160);
        propertyPanelViewPort.setVisible(false);
    }
    
    
}

void MainComponent::mouseMagnify (const juce::MouseEvent &event, float scaleFactor){
    
    scale -= (1-scaleFactor)/2.0;
    
    if(scale <= (getWidth()-10)/(5.0*820.0)) scale = (getWidth()-10)/(5.0*820.0);
    else if(scale >= 2) scale = 2;
    schematic.setTransform(getTransform().scale(scale, scale,event.getPosition().getX(), event.getPosition().getY()));
}


void MainComponent::readAudio(){
    buffer_in.setSize(1, (int)reader->lengthInSamples);
    reader->read(&buffer_in, 0, (int)reader->lengthInSamples, 0, true, false);
}

void MainComponent::processAudio(){
    auto *input = buffer_in.getReadPointer(0);
    buffer_out.setSize(buffer_in.getNumChannels(), buffer_in.getNumSamples());
    auto *output = buffer_out.getWritePointer(0);
    for(auto i=0; i<buffer_in.getNumSamples(); i++){
        //output[i] = wdf->getOutputValue();
        
        if(inputIndex != -1 && inputIndex < leafComponents.size()){
            ((wdfTerminatedResVSource*)(leafComponents[inputIndex]->getWDFComponent()))->Vs = input[i];
        }
        wdfEnvironment.cycleWave();
        
        if(outputIndex != -1 && outputIndex < leafComponents.size()){
            //std::cout << -leafComponents[outputIndex]->getWDFComponent()->upPort->getPortVoltage() << std::endl;
            output[i] = -leafComponents[outputIndex]->getWDFComponent()->upPort->getPortVoltage();
//        std::cout << wdf->getOutputValue() << std::endl;
//        std::cout << input[i] << std::endl;
        }
    }
}

void MainComponent::writeAudio(){
    juce::WavAudioFormat format;
    std::unique_ptr<juce::AudioFormatWriter> writer;
    auto file_out = appDataPath.getChildFile ("output.wav");
    file_out.deleteFile();
    writer.reset(format.createWriterFor(new juce::FileOutputStream(file_out), 44100, buffer_out.getNumChannels(), 24, {}, 0));
    
    if(writer != nullptr){
        std::cout << "audio file written" << std::endl;
        writer->writeFromAudioSampleBuffer(buffer_out, 0, buffer_out.getNumSamples());
    }
}


// File opening and saving starts here  //
juce::StringArray MainComponent::getMenuBarNames()
{
    return { "File", "Edit", "Window","Help" };
}

juce::PopupMenu MainComponent::getMenuForIndex (int menuIndex, const juce::String& /*menuName*/)
{
    juce::PopupMenu menu;
    if (menuIndex == 0)
    {
        menu.addCommandItem (&commandManager, CommandIDs::fileNew);
        menu.addCommandItem (&commandManager, CommandIDs::fileOpen);
       #if JUCE_MAC
        menu.addCommandItem (&commandManager, CommandIDs::fileSave);
       #endif
        menu.addCommandItem (&commandManager, CommandIDs::fileSaveAs);
    }
    else if (menuIndex == 1)
    {
        menu.addCommandItem (&commandManager, CommandIDs::editCopy);
        menu.addCommandItem (&commandManager, CommandIDs::editPaste);
        menu.addCommandItem (&commandManager, CommandIDs::editDelete);
    }
    else if (menuIndex == 2)
    {
        
    }
    return menu;
}

juce::ApplicationCommandTarget* MainComponent::getNextCommandTarget()
{
    return findFirstTargetParentComponent();
}

void MainComponent::getAllCommands (juce::Array<juce::CommandID>& c)
{
    juce::Array<juce::CommandID> commands { CommandIDs::fileNew, CommandIDs::fileOpen,
                                CommandIDs::fileSave,
                                CommandIDs::fileSaveAs, CommandIDs::editCopy };
    c.addArray (commands);
}

void MainComponent::getCommandInfo (juce::CommandID commandID, juce::ApplicationCommandInfo& result)
{
    switch (commandID)
    {
        case CommandIDs::fileNew:
            result.setInfo ("New", "", "Menu", 0);
            result.addDefaultKeypress ('n', juce::ModifierKeys(8));
            break;
        case CommandIDs::fileOpen:
            result.setInfo ("Open", "", "Menu", 0);
            result.addDefaultKeypress ('o', juce::ModifierKeys(8));
            break;
        case CommandIDs::fileSave:
            result.setInfo ("Save", "", "Menu", 0);
            result.addDefaultKeypress ('s', juce::ModifierKeys(8));
            break;
        case CommandIDs::fileSaveAs:
            result.setInfo ("Save As", "", "Menu", 0);
            result.addDefaultKeypress ('s', juce::ModifierKeys(9));
            break;
        case CommandIDs::editCopy:
            result.setInfo ("Copy", "", "Copy", 0);
            result.addDefaultKeypress ('o', juce::ModifierKeys::shiftModifier);
            break;
        default:
            break;
    }
}

bool MainComponent::perform (const InvocationInfo& info) 
{
    juce::File file = appDataPath.getChildFile("circuit.txt");
    
    char lineEnd [] = {'\n',0};
    std::unique_ptr<juce::FileOutputStream> fileOutStream;
    std::unique_ptr<juce::FileInputStream> fileInStream;

    juce::String rslt;
    
    switch (info.commandID)
    {
        case CommandIDs::fileNew:
            leafComponents.clear();
            simpleRoot.reset();
            selectedComponents.clear();
            
            break;
            
        case CommandIDs::fileOpen:
            
            fc.reset(new juce::FileChooser("",appDataPath,"*",true));
            
            fc->launchAsync ( juce::FileBrowserComponent::openMode
                                             | juce::FileBrowserComponent::canSelectFiles,
                                         [this] (const juce::FileChooser& chooser)
                                         {
                                             juce::String chosen;
                                             auto results = chooser.getURLResults();

                                             for (auto result : results)
                                                 chosen << (result.isLocalFile() ? result.getLocalFile().getFullPathName()
                                                                                 : result.toString (false)) << "\n";
                                            
                                            auto file = chooser.getResult();
                                            std::cout << file.getFullPathName() << std::endl;
                if(file.getFileExtension() == ".txt"){
                    auto fileInStream = file.createInputStream();
                    while(fileInStream->isExhausted() != true){
                        std::cout << fileInStream->readNextLine() << std::endl;
                    }
                    
                    juce::String rslt;
                    leafComponents.clear();
                    simpleRoot.reset();
                    
                    fileInStream = file.createInputStream();
                    
                    while(fileInStream->isExhausted() != true){
                        rslt = fileInStream->readNextLine();
                        //rslt.substring(1,)
                        int x1 = rslt.indexOfChar('<');
                        int x2 = rslt.indexOfChar('>');
                        if(x1 != -1 && x2 != -1){
                            int componentTypeIndex = std::find(ComponentTypeStringVector.begin(),ComponentTypeStringVector.end(),rslt.substring(x1+1, x2)) - ComponentTypeStringVector.begin();
                            //std::cout << componentTypeIndex << std::endl;
                            //std::cout << rslt.substring(x1+1, x2) <<  "enum:" << ComponentTypeStringVector[componentTypeIndex] << std::endl;
                            
                            x1 = rslt.indexOf("name");
                            x2 = rslt.indexOfChar('x');
                            juce::String name = rslt.substring(x1+5, x2-1);
                            
                            x1 = rslt.indexOfChar('x');
                            x2 = rslt.indexOfChar('y');
                            int x = rslt.substring(x1+2, x2-1).getIntValue();
                            
                            x1 = rslt.indexOfChar('y');
                            x2 = rslt.indexOf("ang");
                            int y = rslt.substring(x1+2, x2-1).getIntValue();
                            
                            x1 = rslt.indexOf("ang");
                            x2 = rslt.indexOfChar(x1,',');
                            if(x2<0){
                                x2 = rslt.indexOfChar(x1,'<');
                            }
                            int ang = round((rslt.substring(x1+4, x2).getFloatValue())/(juce::MathConstants<float>::halfPi));
                            std::cout << rslt.substring(x1+4, x2) << std::endl;
                            std::cout << ang << std::endl;
                            
                            double val;
                            
                            
                            if(componentTypeIndex != ComponentTypeStringVector.size()){
                                std::function<void(CircuitComponent*c)> componentIsSelectedCallBack = [this] (CircuitComponent*c)
                                {
                                    for(auto c : selectedComponents){
                                        
                                        c->unSelectComponent();
                                    }
                                    selectedComponents.clear();
                                    selectedComponents.push_back(c);
                                };
                                switch(componentTypeIndex){
                                    case ComponentType::L_RES:
                                        x1 = rslt.indexOf("res");
                                        x2 = rslt.indexOfChar(x1,'<');
                                        val = rslt.substring(x1+4, x2).getDoubleValue();
                                        schematic.addAndMakeVisible(leafComponents.add(new Resistor()));
                                        leafComponents.getLast()->setName(name);
                                        ((Resistor*)(leafComponents.getLast()))->setR(val);
                                        if(ang !=0) for(int i=0; i<ang; i++) leafComponents.getLast()->rotateComponent();
                                        leafComponents.getLast()->setBounds(x,y,100,100);
                                        leafComponents.getLast()->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                                        leafComponents.getLast()->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                                        leafComponents.getLast()->componentIsSelectedCallBack = componentIsSelectedCallBack;
                                        break;
                                        
                                    case ComponentType::L_CAP:
                                        x1 = rslt.indexOf("cap");
                                        x2 = rslt.indexOfChar(x1,'<');
                                        val = rslt.substring(x1+4, x2).getDoubleValue();
                                        schematic.addAndMakeVisible(leafComponents.add(new Capacitor()));
                                        leafComponents.getLast()->setName(name);
                                        ((Capacitor*)(leafComponents.getLast()))->setC(val);
                                        if(ang !=0) for(int i=0; i<ang; i++) leafComponents.getLast()->rotateComponent();
                                        leafComponents.getLast()->setBounds(x,y,100,100);
                                        leafComponents.getLast()->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                                        leafComponents.getLast()->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                                        leafComponents.getLast()->componentIsSelectedCallBack = componentIsSelectedCallBack;
                                        break;
                                    case ComponentType::A_SER:
                                        schematic.addAndMakeVisible(leafComponents.add(new Series()));
                                        leafComponents.getLast()->setName(name);
                                        if(ang !=0) for(int i=0; i<ang; i++) leafComponents.getLast()->rotateComponent();
                                        leafComponents.getLast()->setBounds(x,y,100,100);
                                        leafComponents.getLast()->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                                        leafComponents.getLast()->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                                        leafComponents.getLast()->componentIsSelectedCallBack = componentIsSelectedCallBack;
                                        break;
                                    case ComponentType::A_PAR:
                                        schematic.addAndMakeVisible(leafComponents.add(new Parallel()));
                                        leafComponents.getLast()->setName(name);
                                        if(ang !=0) for(int i=0; i<ang; i++) leafComponents.getLast()->rotateComponent();
                                        leafComponents.getLast()->setBounds(x,y,100,100);
                                        leafComponents.getLast()->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                                        leafComponents.getLast()->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                                        leafComponents.getLast()->componentIsSelectedCallBack = componentIsSelectedCallBack;
                                        break;
                                    case ComponentType::L_VOL:
                                        schematic.addAndMakeVisible(leafComponents.add(new VoltageSource()));
                                        leafComponents.getLast()->setName(name);
                                        x1 = rslt.indexOf("vs");
                                        x2 = rslt.indexOfChar(x1,',');
                                        val = rslt.substring(x1+3, x2).getDoubleValue();
                                        ((VoltageSource*)leafComponents.getLast())->setVs(val);
                                        x1 = rslt.indexOf("rs");
                                        x2 = rslt.indexOfChar(x1,'<');
                                        val = rslt.substring(x1+3, x2).getDoubleValue();
                                        ((VoltageSource*)leafComponents.getLast())->setRs(val);
                                        if(ang !=0) for(int i=0; i<ang; i++) leafComponents.getLast()->rotateComponent();
                                        leafComponents.getLast()->setBounds(x,y,100,100);
                                        leafComponents.getLast()->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                                        leafComponents.getLast()->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                                        leafComponents.getLast()->componentIsSelectedCallBack = componentIsSelectedCallBack;
                                        break;
                                    case ComponentType::A_INV:
                                        schematic.addAndMakeVisible(leafComponents.add(new Inverter()));
                                        leafComponents.getLast()->setName(name);
                                        if(ang !=0) for(int i=0; i<ang; i++) leafComponents.getLast()->rotateComponent();
                                        leafComponents.getLast()->setBounds(x,y,100,100);
                                        leafComponents.getLast()->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                                        leafComponents.getLast()->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                                        leafComponents.getLast()->componentIsSelectedCallBack = componentIsSelectedCallBack;
                                        break;
                                    case ComponentType::SR_SHC:
                                        simpleRoot = std::make_unique<ShortCircuit>();
                                        simpleRoot->setName(name);
                                        schematic.addAndMakeVisible(simpleRoot.get());
                                        if(ang !=0) for(int i=0; i<ang; i++) simpleRoot->rotateComponent();
                                        simpleRoot->setBounds(x,y,100,100);
                                        simpleRoot->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                                        simpleRoot->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                                        simpleRoot->componentIsSelectedCallBack = componentIsSelectedCallBack;
                                        wantsToConnect_(simpleRoot.get());
                                        break;
                                    case ComponentType::SR_VOL:
                                        simpleRoot = std::make_unique<IdealVoltageSource>();
                                        simpleRoot->setName(name);
                                        schematic.addAndMakeVisible(simpleRoot.get());
                                        if(ang !=0) for(int i=0; i<ang; i++) simpleRoot->rotateComponent();
                                        simpleRoot->setBounds(x,y,100,100);
                                        simpleRoot->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                                        simpleRoot->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                                        simpleRoot->componentIsSelectedCallBack = componentIsSelectedCallBack;
                                        wantsToConnect_(simpleRoot.get());
                                        break;
                                    case ComponentType::R_RNODE:
                                        x1 = rslt.indexOf("collums");
                                        x2 = rslt.indexOfChar(x1,'<');
                                        rNode = std::make_unique<RNodeRootComponent>(rslt.substring(x1, x2));
                                        rNode->setName(name);
                                        schematic.addAndMakeVisible(rNode.get());
                                        if(ang !=0) for(int i=0; i<ang; i++) rNode->rotateComponent();
                                        rNode->setTopLeftPosition(x, y);
                                        rNode->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                                        rNode->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                                        rNode->componentIsSelectedCallBack = componentIsSelectedCallBack;
                                        wantsToConnect_(rNode.get());
                                        break;
                                }
                                if(leafComponents.size()>0) wantsToConnect_(leafComponents.getLast());
                            }
                            
                        }
                        //std::cout << fileInStream->readNextLine() << std::endl;
                    }
                    
                    
                }
                else std::cout << "file can't be opened" << std::endl;
            });
            
            
            
//            leafComponents.clear();
//
//            fileInStream = file.createInputStream();
//
//            while(fileInStream->isExhausted() != true){
//                rslt = fileInStream->readNextLine();
//                //rslt.substring(1,)
//                int x1 = rslt.indexOfChar('<');
//                int x2 = rslt.indexOfChar('>');
//                if(x1 != -1 && x2 != -1){
//                    int componentTypeIndex = std::find(ComponentTypeStringVector.begin(),ComponentTypeStringVector.end(),rslt.substring(x1+1, x2)) - ComponentTypeStringVector.begin();
//                    //std::cout << componentTypeIndex << std::endl;
//                    //std::cout << rslt.substring(x1+1, x2) <<  "enum:" << ComponentTypeStringVector[componentTypeIndex] << std::endl;
//
//                    x1 = rslt.indexOf("name");
//                    x2 = rslt.indexOfChar('x');
//                    juce::String name = rslt.substring(x1+5, x2-1);
//
//                    x1 = rslt.indexOfChar('x');
//                    x2 = rslt.indexOfChar('y');
//                    int x = rslt.substring(x1+2, x2-1).getIntValue();
//
//                    x1 = rslt.indexOfChar('y');
//                    x2 = rslt.indexOf("ang");
//                    int y = rslt.substring(x1+2, x2-1).getIntValue();
//
//                    x1 = rslt.indexOf("ang");
//                    x2 = rslt.indexOfChar(x1,',');
//                    if(x2<0){
//                        x2 = rslt.indexOfChar(x1,'<');
//                    }
//                    int ang = round((rslt.substring(x1+4, x2).getFloatValue())/(juce::MathConstants<float>::halfPi));
//                    std::cout << rslt.substring(x1+4, x2) << std::endl;
//                    std::cout << ang << std::endl;
//
//                    double val;
//
//
//                    if(componentTypeIndex != ComponentTypeStringVector.size()){
//                        std::function<void(CircuitComponent*c)> componentIsSelectedCallBack = [this] (CircuitComponent*c)
//                        {
//                            for(auto c : selectedComponents){
//
//                                c->unSelectComponent();
//                            }
//                            selectedComponents.clear();
//                            selectedComponents.push_back(c);
//                        };
//                        switch(componentTypeIndex){
//                            case ComponentType::L_RES:
//                                x1 = rslt.indexOf("res");
//                                x2 = rslt.indexOfChar(x1,'<');
//                                val = rslt.substring(x1+4, x2).getDoubleValue();
//                                schematic.addAndMakeVisible(leafComponents.add(new Resistor()));
//                                leafComponents.getLast()->setName(name);
//                                ((Resistor*)(leafComponents.getLast()))->setR(val);
//                                if(ang !=0) for(int i=0; i<ang; i++) leafComponents.getLast()->rotateComponent();
//                                leafComponents.getLast()->setBounds(x,y,100,100);
//                                leafComponents.getLast()->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
//                                leafComponents.getLast()->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
//                                leafComponents.getLast()->componentIsSelectedCallBack = componentIsSelectedCallBack;
//                                break;
//
//                            case ComponentType::L_CAP:
//                                x1 = rslt.indexOf("cap");
//                                x2 = rslt.indexOfChar(x1,'<');
//                                val = rslt.substring(x1+4, x2).getDoubleValue();
//                                schematic.addAndMakeVisible(leafComponents.add(new Capacitor()));
//                                leafComponents.getLast()->setName(name);
//                                ((Capacitor*)(leafComponents.getLast()))->setC(val);
//                                if(ang !=0) for(int i=0; i<ang; i++) leafComponents.getLast()->rotateComponent();
//                                leafComponents.getLast()->setBounds(x,y,100,100);
//                                leafComponents.getLast()->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
//                                leafComponents.getLast()->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
//                                leafComponents.getLast()->componentIsSelectedCallBack = componentIsSelectedCallBack;
//                                break;
//                            case ComponentType::A_SER:
//                                schematic.addAndMakeVisible(leafComponents.add(new Series()));
//                                leafComponents.getLast()->setName(name);
//                                if(ang !=0) for(int i=0; i<ang; i++) leafComponents.getLast()->rotateComponent();
//                                leafComponents.getLast()->setBounds(x,y,100,100);
//                                leafComponents.getLast()->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
//                                leafComponents.getLast()->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
//                                leafComponents.getLast()->componentIsSelectedCallBack = componentIsSelectedCallBack;
//                                break;
//                            case ComponentType::A_PAR:
//                                schematic.addAndMakeVisible(leafComponents.add(new Parallel()));
//                                leafComponents.getLast()->setName(name);
//                                if(ang !=0) for(int i=0; i<ang; i++) leafComponents.getLast()->rotateComponent();
//                                leafComponents.getLast()->setBounds(x,y,100,100);
//                                leafComponents.getLast()->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
//                                leafComponents.getLast()->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
//                                leafComponents.getLast()->componentIsSelectedCallBack = componentIsSelectedCallBack;
//                                break;
//                            case ComponentType::L_VOL:
//                                schematic.addAndMakeVisible(leafComponents.add(new VoltageSource()));
//                                leafComponents.getLast()->setName(name);
//                                x1 = rslt.indexOf("vs");
//                                x2 = rslt.indexOfChar(x1,',');
//                                val = rslt.substring(x1+3, x2).getDoubleValue();
//                                ((VoltageSource*)leafComponents.getLast())->setVs(val);
//                                x1 = rslt.indexOf("rs");
//                                x2 = rslt.indexOfChar(x1,'<');
//                                val = rslt.substring(x1+3, x2).getDoubleValue();
//                                ((VoltageSource*)leafComponents.getLast())->setRs(val);
//                                if(ang !=0) for(int i=0; i<ang; i++) leafComponents.getLast()->rotateComponent();
//                                leafComponents.getLast()->setBounds(x,y,100,100);
//                                leafComponents.getLast()->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
//                                leafComponents.getLast()->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
//                                leafComponents.getLast()->componentIsSelectedCallBack = componentIsSelectedCallBack;
//                                break;
//                            case ComponentType::SR_SHC:
//                                simpleRoot = std::make_unique<ShortCircuit>();
//                                simpleRoot->setName("Shrt1");
//                                schematic.addAndMakeVisible(simpleRoot.get());
//                                if(ang !=0) for(int i=0; i<ang; i++) simpleRoot->rotateComponent();
//                                simpleRoot->setBounds(x,y,100,100);
//                                simpleRoot->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
//                                simpleRoot->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
//                                simpleRoot->componentIsSelectedCallBack = componentIsSelectedCallBack;
//                                wantsToConnect_(simpleRoot.get());
//                                break;
//                            case ComponentType::SR_VOL:
//                                simpleRoot = std::make_unique<IdealVoltageSource>();
//                                simpleRoot->setName("V1");
//                                schematic.addAndMakeVisible(simpleRoot.get());
//                                if(ang !=0) for(int i=0; i<ang; i++) simpleRoot->rotateComponent();
//                                simpleRoot->setBounds(x,y,100,100);
//                                simpleRoot->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
//                                simpleRoot->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
//                                simpleRoot->componentIsSelectedCallBack = componentIsSelectedCallBack;
//                                wantsToConnect_(simpleRoot.get());
//                                break;
//                        }
//                        if(leafComponents.size()>0) wantsToConnect_(leafComponents.getLast());
//                    }
//
//                }
//                //std::cout << fileInStream->readNextLine() << std::endl;
//            }
            
            
            
            
            break;
        case CommandIDs::fileSave:
            file.deleteFile();
            fileOutStream = file.createOutputStream();
            
            for(auto l: leafComponents){
                //if(l->isComponentConnected()==false){
                    std::cout << l->getInfo() << std::endl;
                    if(fileOutStream != nullptr)fileOutStream->writeText(l->getInfo()+"\n", false, false,lineEnd);
                //}
            }
            if(simpleRoot != nullptr){
                std::cout << simpleRoot->getInfo();
                if(fileOutStream != nullptr)fileOutStream->writeText(simpleRoot->getInfo()+"\n", false, false,lineEnd);
            }
            
            if(rNode != nullptr){
                std::cout << rNode->getInfo();
                if(fileOutStream != nullptr)fileOutStream->writeText(rNode->getInfo()+"\n", false, false,lineEnd);
            }
            
            fileOutStream.reset();
            break;
        case CommandIDs::fileSaveAs:
            
            fc.reset(new juce::FileChooser("",appDataPath,"*",true));
            
            fc->launchAsync ( juce::FileBrowserComponent::saveMode
                                             | juce::FileBrowserComponent::canSelectFiles,
                                         [this] (const juce::FileChooser& chooser)
                                         {
                                             juce::String chosen;
                                             auto results = chooser.getURLResults();

                                             for (auto result : results)
                                                 chosen << (result.isLocalFile() ? result.getLocalFile().getFullPathName()
                                                                                 : result.toString (false)) << "\n";
                                            
                                            auto file = chooser.getResult();
                                            std::cout << file.getFullPathName() << std::endl;
                if(file.getFileExtension() == ".txt"){
//                    auto fileOutStream = file.createOutputStream();
//                    if(fileOutStream != nullptr)fileOutStream->writeText(l1.getText(), false, false,nullptr);
//                    fileOutStream.reset();
                }
                else std::cout << "file can't be opened" << std::endl;
                });
            
            
            
            break;
        case CommandIDs::editCopy:
//            controlPanel.setBounds(0, getHeight()-30, getWidth(), 30);
//            frontPanel.setBounds(0,getHeight()-250,getWidth(),250 - 30);
//
//            if(propertyPanelShowHide){
//                viewPort.setBounds(0, 0, getWidth()-250, getHeight()-160);
//                propertyPanelViewPort.setBounds(getWidth()-250,0,250,getHeight()-160);
//                propertyPanelViewPort.setVisible(true);
//            }
//            else{
//                viewPort.setBounds(0, 0, getWidth(), getHeight()-250);
//                propertyPanelViewPort.setVisible(false);
//            }
            
            if(rNode != nullptr) std::cout << rNode->getInfo() << std::endl;
            
            break;
        default:
            return false;
    }
    return true;
}

bool MainComponent::keyPressed(const juce::KeyPress &key, juce::Component * originatingComponent){
    if(key.isKeyCode(juce::KeyPress::backspaceKey)){
        std::cout << "delete pressed " << std::endl;
        for(auto c : selectedComponents){
            if(leafComponents.contains((AdaptedLeafComponent*)c)) {
                leafComponents.removeObject((AdaptedLeafComponent*)c);
            }
            if(c == simpleRoot.get()) simpleRoot.reset();
        }
        selectedComponents.clear();
    }
    if(key.isKeyCode(juce::KeyPress::escapeKey)){
        for(auto c: selectedComponents){
            c->unSelectComponent();
        }
        selectedComponents.clear();
    }
    return true;
}
